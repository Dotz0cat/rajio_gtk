#include "rajio.h"
//clang is loud
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

//marcos
#define sql "/home/seth/c/rajio_gtk_pack_test/stations"

//prototypes
void add_station(GtkWidget* flowbox, char* station_name, char* image_file);
void station_adder(char* file_name, GtkWidget* flow);
static void error_message_popup(GtkWidget* parrent, char* error_message);
int start_playing(int station_id);
int stop_playing(void);
GtkWidget* make_image_from_file(char* file, int x, int y);
void change_station_playing_image(char* thumbnail);
GtkWidget* make_image_from_resource(const char* address, int x, int y);
GInputStream* make_input_stream(const char* address);

//gtk callback prototypes
static void destroy(GtkWidget *widget, gpointer data);
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
static void button_clicked_cb(GtkWidget *widget, gpointer data);
static void event_box_clicked_cb(GtkWidget *widget, gpointer data);
static void file_chooser_thumbnail_clicked_cb(GtkWidget *widget, gpointer data);
static void file_chooser_address_clicked_cb(GtkWidget *widget, gpointer parrent);
static void stop_button_clicked_cb(GtkWidget* widget, gpointer data);
static void play_button_clicked_cb(GtkWidget* widget, gpointer data);
static void pause_button_clicked_cb(GtkWidget* widget, gpointer data);
static void image_loaded_cb(GObject* source_object, GAsyncResult* res, gpointer user_data);

//external prototypes
extern int get_highest_id(char* file_name);
extern char* read_station_name(char* file_name, int id);
extern char* read_station_thumbnail(char* file_name, int id);
extern int append_new_station(char* file_name, int id, char* name, char* thumbnail, int num_of_addresses);
extern int append_new_address(char* file_name, int id, char* address);
extern char* get_address(char* file_name, int id);
extern void set_message_handlers(GstBus *bus, const char* sql_file);

//external parser prototypes
extern int add_stations(char* file_name, char* sql_file);
extern int is_valid_url(char* url);
extern int contains_a_pls(char* url);
extern char* get_address_from_pls_over_net(char* pls_file);
extern int genaric_regex(const char* string, const char* regex_string);

//global variables
static int station_number;
static GtkWidget* station_image;
static GtkWidget* station_name_label;
static GtkWidget* play_button;
static GtkWidget* stop_button;
static GtkWidget* pause_button;
//add pause button
int most_recent_id;
GstElement* pipeline;

int main(int argc, char* argv[]) {

    GtkBuilder* builder;
    GtkWidget* window;
    GtkWidget* flow;

    GstBus* bus;

    gtk_init(&argc, &argv);

    gst_init(&argc, &argv);

    pipeline = gst_element_factory_make("playbin", NULL);

    gst_element_set_state(pipeline, GST_STATE_READY);

    bus = gst_element_get_bus(pipeline);

    set_message_handlers(bus, sql);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "rajio_gtk_v2.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "gWindow"));
    flow = GTK_WIDGET(gtk_builder_get_object(builder, "flowbox1"));
    station_image = GTK_WIDGET(gtk_builder_get_object(builder, "gStaationPlaying"));
    station_name_label = GTK_WIDGET(gtk_builder_get_object(builder, "gStationNamePlaying"));
    play_button = GTK_WIDGET(gtk_builder_get_object(builder, "gPlay"));
    stop_button = GTK_WIDGET(gtk_builder_get_object(builder, "gStop"));
    pause_button = GTK_WIDGET(gtk_builder_get_object(builder, "gPause"));

    //add the staion images and names to the flowbox
    station_adder(sql, flow);

    //add a dioluge popup to the add station button
    GtkWidget* station_add;
    station_add = GTK_WIDGET(gtk_builder_get_object(builder, "gStationAdd"));

    g_signal_connect(station_add, "clicked", G_CALLBACK(button_clicked_cb), (gpointer) window);
    g_signal_connect(play_button, "clicked", G_CALLBACK(play_button_clicked_cb), (gpointer) window);
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_button_clicked_cb), (gpointer) window);
    g_signal_connect(pause_button, "clicked", G_CALLBACK(pause_button_clicked_cb), (gpointer) window);



    //makes the window close
    g_signal_connect(window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK (destroy), NULL);

    gtk_widget_show_all(window);
    gtk_widget_hide(play_button);
    gtk_widget_hide(stop_button);
    gtk_widget_hide(pause_button);
    gtk_main();

    return 0;
}

void add_station(GtkWidget* flowbox, char* station_name, char* image_file) {
    GtkWidget* grid;

    grid = gtk_grid_new();

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    //make the gtkImage
    GtkWidget* image;
    //image = make_image_from_file(image_file, 100, 100);
    image = make_image_from_resource(image_file, 100, 100);

    gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 3, 2);

    //make the label
    GtkWidget* label;
    label = gtk_label_new(station_name);

    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 3, 1);

    //wrap the grid in a event box
    GtkWidget* event_box;

    event_box = gtk_event_box_new();

    char str[50];
    sprintf(str, "%i", station_number);

    gtk_widget_set_name(event_box, str);

    g_signal_connect(event_box, "button_press_event", G_CALLBACK(event_box_clicked_cb), NULL);

    gtk_container_add(GTK_CONTAINER(event_box), grid);

    //need to add some event handllers to the event box

    //add the event box to the flowbox
    gtk_container_add(GTK_CONTAINER(flowbox), event_box);

    //icremnt the station number
    station_number++;
}

void station_adder(char* file_name, GtkWidget* flow) {
    int max_station = get_highest_id(file_name);

    char* name;
    char* path;

    for (station_number = 1; station_number < max_station+1;) {
        name = read_station_name(file_name, station_number);
        path = read_station_thumbnail(file_name, station_number);
        add_station(flow, name, path);
        free(name);
        free(path);
    }
}

static void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    return FALSE; 
}

static void button_clicked_cb(GtkWidget *widget, gpointer data) {
    GtkWidget* diolouge;

    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

    diolouge = gtk_dialog_new_with_buttons("Add Station", data, flags, "Ok", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_REJECT, NULL);

    //make a grid and stuff and add stuff to it
    GtkWidget* content_area;

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(diolouge));

    //grid
    GtkWidget* grid;

    grid = gtk_grid_new();

    gtk_widget_set_name(grid, "add_station_grid");

    //delectations
    GtkWidget* name;
    GtkWidget* name_entry;
    GtkWidget* thumbnail;
    GtkWidget* thumbnail_entry;
    GtkWidget* thumbnail_chooser;
    GtkWidget* address_label;
    GtkWidget* radio_use_file;
    GtkWidget* radio_enter_manually;
    GtkWidget* address_file_entry;
    GtkWidget* address_file_chooser;
    GtkWidget* address_manual_entry;

    //useful for later
    const char* name_value;
    const char* thumbnail_path;
    int num_of_addresses;

    //make for name and add to grid
    name = gtk_label_new("Name");

    gtk_grid_attach(GTK_GRID(grid), name, 2, 0, 1, 1);

    //make name entry box
    name_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 4, 1);

    //make thumbnail label
    thumbnail = gtk_label_new("Thumnail");

    gtk_grid_attach(GTK_GRID(grid), thumbnail, 2, 4, 1, 1);

    //make thumbnail entry
    thumbnail_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), thumbnail_entry, 1, 5, 3, 1);

    //add file chooser button for the thumbnail entry
    //make a file chooser dialog popup

    thumbnail_chooser = gtk_button_new();
    // dont know how to make it look right

    //add singal
    g_signal_connect(thumbnail_chooser, "clicked", G_CALLBACK(file_chooser_thumbnail_clicked_cb), (gpointer) diolouge);

    gtk_grid_attach(GTK_GRID(grid), thumbnail_chooser, 4, 5, 1, 1);

    //add a label
    address_label = gtk_label_new("Address");

    gtk_grid_attach(GTK_GRID(grid), address_label, 2, 7, 1, 1);

    //add first radio button
    radio_use_file = gtk_radio_button_new(NULL);

    //make address file entry
    address_file_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), radio_use_file, 0, 8, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), address_file_entry, 1, 8, 3, 1);

    //add the button for file selection
    address_file_chooser = gtk_button_new();

    g_signal_connect(address_file_chooser, "clicked", G_CALLBACK(file_chooser_address_clicked_cb), (gpointer) diolouge);

    gtk_grid_attach(GTK_GRID(grid), address_file_chooser, 4, 8, 1, 1);

    //add other radio button
    radio_enter_manually = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio_use_file));

    //entry box
    address_manual_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), radio_enter_manually, 0, 9, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), address_manual_entry, 1, 9, 4, 1);
    //add grid to content area
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    //show gtk stuff and enter a loop
    gtk_widget_show_all(diolouge);

    int response = gtk_dialog_run(GTK_DIALOG(diolouge));

    switch (response) {
        case(GTK_RESPONSE_ACCEPT):
            //get info and make call to append_new_station() and append_new_address
            name_value = (const char*) gtk_entry_get_text(GTK_ENTRY(name_entry));
            if (strcmp(name_value, "") == 0) {
                error_message_popup(diolouge, "Please enter a valid name");
            }

            thumbnail_path = (const char*) gtk_entry_get_text(GTK_ENTRY(thumbnail_entry));
            if (strcmp(thumbnail_path, "") == 0) {
                error_message_popup(diolouge, "Please enter a valid thumbnail path");
            }

            num_of_addresses = 0;

            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_enter_manually))) {
                
                const char* address = (const char*) gtk_entry_get_text(GTK_ENTRY(address_manual_entry));

                if (is_valid_url(address) == 0) {
                    num_of_addresses = 1;
                    if (append_new_address(sql, (get_highest_id(sql)+1), address) != 0) {
                        fprintf(stderr, "There was a error adding a address to the table\r\n");
                    }
                }
                else {
                    error_message_popup(diolouge, "The address entered was not valid");
                }
            }
            else {
                //the use file button is toggled
                const char* address = (const char*) gtk_entry_get_text(GTK_ENTRY(address_file_entry));
                num_of_addresses = add_stations(address, sql);
                if (num_of_addresses <= 0) {
                    error_message_popup(diolouge, "There was a error parsing the file provided\r\nor the file was not valid");
                }
            }

            if (num_of_addresses <= 0) {
                error_message_popup(diolouge, "There was a issue somewhere");
            }

            if (append_new_station(sql, get_highest_id(sql)+1, name_value, thumbnail_path, num_of_addresses) != 0) {
                error_message_popup(diolouge, "There was a error adding the station");
            }
        break;
        default:

        break;
    }
    gtk_widget_destroy(diolouge);
}

static void event_box_clicked_cb(GtkWidget *widget, gpointer data) {
    char* id;
    id = gtk_widget_get_name(widget);

    int id_num = atoi(id);

    most_recent_id = id_num;

    //get the gtk window
    GtkWidget* flow_child = gtk_widget_get_parent(widget);
    GtkWidget* flow = gtk_widget_get_parent(flow_child);
    GtkWidget* view = gtk_widget_get_parent(flow);
    GtkWidget* scroll = gtk_widget_get_parent(view);
    GtkWidget* fixed = gtk_widget_get_parent(scroll);
    GtkWidget* window = gtk_widget_get_parent(fixed);

    if (start_playing(id_num) != 0) {
        error_message_popup(window, "There was a error somewhere");
        return;
    }

    return;
}

//this is for the thumbnail
static void file_chooser_thumbnail_clicked_cb(GtkWidget *widget, gpointer parrent) {
    GtkWidget* dialog;
    GtkWidget* entry;

    //intermediate
    GtkWidget* grid;
    GtkWidget* box;
    GList* list;

    box = gtk_bin_get_child(GTK_BIN(parrent));

    grid = NULL;

    list = gtk_container_get_children(GTK_CONTAINER(box));

    if (g_ascii_strcasecmp(gtk_widget_get_name((GtkWidget*) list->data), "add_station_grid") == 0) {
        grid = (GtkWidget*) list->data;
    }
    else {
        while((list = g_list_next(list)) != NULL) {
            if (g_ascii_strcasecmp(gtk_widget_get_name((GtkWidget*) list->data), "add_station_grid") == 0) {
                grid = (GtkWidget*) list->data;
            }

            if (grid != NULL) break;
        }
    }

    entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 5);

    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new("Open File", parrent, action, "Cancel", GTK_RESPONSE_CANCEL, "Ok", GTK_RESPONSE_ACCEPT, NULL);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        char* filename;

        GtkFileChooser* chooser;

        chooser = GTK_FILE_CHOOSER(dialog);

        filename = gtk_file_chooser_get_filename(chooser);

        gtk_entry_set_text(GTK_ENTRY(entry), filename);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

//this is for the address
static void file_chooser_address_clicked_cb(GtkWidget *widget, gpointer parrent) {
    GtkWidget* dialog;
    GtkWidget* entry;

    //intermediate
    GtkWidget* grid;
    GtkWidget* box;
    GList* list;

    box = gtk_bin_get_child(GTK_BIN(parrent));

    grid = NULL;

    list = gtk_container_get_children(GTK_CONTAINER(box));

    if (g_ascii_strcasecmp(gtk_widget_get_name((GtkWidget*) list->data), "add_station_grid") == 0) {
        grid = (GtkWidget*) list->data;
    }
    else {
        while((list = g_list_next(list)) != NULL) {
            if (g_ascii_strcasecmp(gtk_widget_get_name((GtkWidget*) list->data), "add_station_grid") == 0) {
                grid = (GtkWidget*) list->data;
            }

            if (grid != NULL) break;
        }
    }

    entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 8);

    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new("Open File", parrent, action, "Cancel", GTK_RESPONSE_CANCEL, "Ok", GTK_RESPONSE_ACCEPT, NULL);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        char* filename;

        GtkFileChooser* chooser;

        chooser = GTK_FILE_CHOOSER(dialog);

        filename = gtk_file_chooser_get_filename(chooser);

        gtk_entry_set_text(GTK_ENTRY(entry), filename);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void error_message_popup(GtkWidget* parrent, char* error_message) {
    GtkWidget* dialog;
    GtkWidget* label;
    GtkWidget* content_area;

    //GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

    dialog = gtk_dialog_new_with_buttons("ERROR", GTK_WINDOW(parrent), NULL, "Ok", GTK_RESPONSE_NONE, NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    label = gtk_label_new(error_message);

    gtk_container_add(GTK_CONTAINER(content_area), label);

    gtk_widget_show_all(dialog);

    (void) gtk_dialog_run(GTK_DIALOG(dialog));
    //when this is done just quit and destroy the dialog and the parent

    gtk_widget_destroy(dialog);

    //gtk_widget_destroy(parrent);

}

int start_playing(int station_id) {
    char* address = get_address(sql, station_id);

    char address_stack[512];

    strcpy(address_stack, address);

    free(address);

    char* thumbnail;

    thumbnail = read_station_thumbnail(sql, station_id);

    char* name;

    name = read_station_name(sql, station_id);

    gst_element_set_state(pipeline, GST_STATE_READY);

    if (is_valid_url(address_stack) == 0 && contains_a_pls(address_stack) == 0) {
            char* true_address;
            true_address = get_address_from_pls_over_net(address_stack);

            if (is_valid_url(true_address) != 0) return -1;

            g_object_set(pipeline, "uri", true_address, NULL);

            free(true_address);

    }
    else if (is_valid_url(address_stack) == 0) {

        g_object_set(pipeline, "uri", address_stack, NULL);

    }
    else return -1;

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    gtk_widget_show(stop_button);
    gtk_widget_show(pause_button);
    gtk_widget_hide(play_button);
    change_station_playing_image(thumbnail);
    gtk_label_set_text(GTK_LABEL(station_name_label), name);

    free(thumbnail);
    free(name);

    return 0;
}

int stop_playing() {

    gst_element_set_state(pipeline, GST_STATE_READY);

    gtk_widget_show(play_button);
    gtk_widget_hide(stop_button);
    gtk_widget_hide(pause_button);
    gtk_image_set_from_icon_name(GTK_IMAGE(station_image), "audio-x-generic", GTK_ICON_SIZE_BUTTON);
    gtk_label_set_text(GTK_LABEL(station_name_label), "No Station Playing");

    return 0;
}

static void stop_button_clicked_cb(GtkWidget* widget, gpointer data) {
    if (stop_playing() != 0) {
        error_message_popup(data, "There was a error with stoping");
    }
}

static void play_button_clicked_cb(GtkWidget* widget, gpointer data) {

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    char* thumbnail;

    thumbnail = read_station_thumbnail(sql, most_recent_id);

    char* name;

    name = read_station_name(sql, most_recent_id);

    gtk_widget_show(stop_button);
    gtk_widget_show(pause_button);
    gtk_widget_hide(play_button);
    change_station_playing_image(thumbnail);
    gtk_label_set_text(GTK_LABEL(station_name_label), name);

    free(thumbnail);
    free(name);
}

static void pause_button_clicked_cb(GtkWidget* widget, gpointer data) {
    gst_element_set_state(pipeline, GST_STATE_PAUSED);

    gtk_label_set_text(GTK_LABEL(station_name_label), "Paused");

    gtk_widget_show(play_button);
    gtk_widget_show(stop_button);
    gtk_widget_hide(pause_button);
}

GtkWidget* make_image_from_file(char* file, int x, int y) {
    GtkWidget* image;
    GdkPixbuf* pixbuf;

    //GError* error = NULL;

    pixbuf = gdk_pixbuf_new_from_file_at_scale(file, x, y, FALSE, NULL);

    //fprintf(stderr, "error with pixbuf: %s\r\n", error->message);

    image = gtk_image_new_from_pixbuf(pixbuf);

    g_object_unref(pixbuf);

    return image;
}

void change_station_playing_image(char* thumbnail) {
    GdkPixbuf* pixbuf;

    pixbuf = gdk_pixbuf_new_from_file_at_scale(thumbnail, 50, 50, FALSE, NULL);

    gtk_image_set_from_pixbuf(GTK_IMAGE(station_image), pixbuf);

    g_object_unref(pixbuf);
}

GtkWidget* make_image_from_resource(const char* address, int x, int y) {
    GtkWidget* image;

    GInputStream* stream = make_input_stream(address);

    gdk_pixbuf_new_from_stream_at_scale_async(stream, x, y, FALSE, NULL, image_loaded_cb, image);

    image = gtk_image_new_from_icon_name("audio-x-generic", GTK_ICON_SIZE_BUTTON);

    return image;
}

static void image_loaded_cb(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GdkPixbuf* pixbuf;

    pixbuf = gdk_pixbuf_new_from_stream_finish(res, NULL);

    gtk_image_set_from_pixbuf(GTK_IMAGE(user_data), pixbuf);

    g_object_unref(pixbuf);
}

GInputStream* make_input_stream(const char* address) {
    GFile* fp = g_file_new_for_uri(address);
    GFileInputStream* stream = g_file_read(fp, NULL, NULL);

    if (!stream) {
        g_object_unref(fp);
        return NULL;
    }

    return G_INPUT_STREAM(stream);
}
