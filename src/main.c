/*
Copyright 2020 Dotz0cat

This file is part of Rajio.

    Rajio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rajio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rajio.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "rajio.h"
//clang is loud
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

//marcos
#ifndef stations_file
    #define stations_file "/usr/local/share/rajio/stations"
#endif

#ifndef gtk_builder_file
    #define gtk_builder_file "/usr/local/share/rajio/rajio_gtk_v2.glade"
#endif

//prototypes
void add_station(GtkWidget* flowbox, char* station_name, char* image_file, int id, CatStationFile station_file);
void station_adder(char* file_name, GtkWidget* flow, CatStationFile station_file);
static void error_message_popup(GtkWidget* parrent, char* error_message);
int start_playing(int station_id, CatStationFile file);
int stop_playing(void);
void change_station_playing_image(char* thumbnail);
GtkWidget* make_image_from_resource(const char* address, int x, int y);
GInputStream* make_input_stream(const char* address);
void localDB(void);
void eos_changer(void);
int start_playing_with_reroll(int station_id, int reroll, CatStationFile file);

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
static void file_read_cb(GObject* source_object, GAsyncResult* res, gpointer user_data);

//external prototypes
extern int get_highest_id(char* file_name);
extern char* read_station_name(char* file_name, int id);
extern char* read_station_thumbnail(char* file_name, int id);
extern int append_new_station(char* file_name, int id, char* name, char* thumbnail, int num_of_addresses);
extern int append_new_address(char* file_name, int id, char* address);
extern char* get_address(char* file_name, int id);
extern void set_message_handlers(GstBus* bus);
extern int local_exsits(const char* file_name);
extern int makeDB(const char* file_name);
extern char* address_reroll(const char* file_name, int station_id, int reroll);
extern int get_num_of_addresses(const char* file_name, int id);

//external parser prototypes
extern int add_stations(char* file_name, char* sql_file);
extern int is_valid_url(char* url);
extern int contains_a_pls(char* url);
extern char* get_address_from_pls_over_net(char* pls_file);
extern int genaric_regex(const char* string, const char* regex_string);

//global variables
//static int station_number;
static GtkWidget* station_image;
static GtkWidget* station_name_label;
static GtkWidget* play_button;
static GtkWidget* stop_button;
static GtkWidget* pause_button;
//add pause button
int most_recent_id;
CatStationFile most_recent_file;
int most_recent_reroll;
GstElement* pipeline;
char* local_station_file;

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

    set_message_handlers(bus);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, gtk_builder_file, NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "gWindow"));
    flow = GTK_WIDGET(gtk_builder_get_object(builder, "flowbox1"));
    station_image = GTK_WIDGET(gtk_builder_get_object(builder, "gStaationPlaying"));
    station_name_label = GTK_WIDGET(gtk_builder_get_object(builder, "gStationNamePlaying"));
    play_button = GTK_WIDGET(gtk_builder_get_object(builder, "gPlay"));
    stop_button = GTK_WIDGET(gtk_builder_get_object(builder, "gStop"));
    pause_button = GTK_WIDGET(gtk_builder_get_object(builder, "gPause"));

    //check and make local db
    localDB();

    //add the staion images and names to the flowbox
    station_adder(stations_file, flow, SYSTEM);
    station_adder(local_station_file, flow, LOCAL);

    //add a dioluge popup to the add station button
    GtkWidget* station_add;
    station_add = GTK_WIDGET(gtk_builder_get_object(builder, "gStationAdd"));

    g_signal_connect(station_add, "clicked", G_CALLBACK(button_clicked_cb), (gpointer) window);
    g_signal_connect(play_button, "clicked", G_CALLBACK(play_button_clicked_cb), (gpointer) window);
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_button_clicked_cb), (gpointer) window);
    g_signal_connect(pause_button, "clicked", G_CALLBACK(pause_button_clicked_cb), (gpointer) window);

    //gtk_button_set_relief(GTK_BUTTON(station_add), GTK_RELIEF_NONE);

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

void add_station(GtkWidget* flowbox, char* station_name, char* image_file, int id, CatStationFile station_file) {
    GtkWidget* grid;

    grid = gtk_grid_new();

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    //make the gtkImage
    GtkWidget* image;

    image = make_image_from_resource(image_file, 100, 100);

    gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 3, 2);

    //make the label
    GtkWidget* label;
    label = gtk_label_new(station_name);

    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 3, 1);

    GtkWidget* button;

    button = cat_station_button_new();

    cat_station_button_set_id(button, id);
    cat_station_button_set_station_file(button, station_file);

    g_signal_connect(GTK_BUTTON(button), "button_press_event", G_CALLBACK(event_box_clicked_cb), NULL);

    gtk_container_add(GTK_CONTAINER(button), grid);

    //add the station button to the flowbox
    gtk_container_add(GTK_CONTAINER(flowbox), button);

    gtk_widget_show_all(button);
}

void station_adder(char* file_name, GtkWidget* flow, CatStationFile station_file) {
    int max_station = get_highest_id(file_name);

    char* name;
    char* path;

    for (int i = 1; i < max_station+1; i++) {
        name = read_station_name(file_name, i);
        path = read_station_thumbnail(file_name, i);
        add_station(flow, name, path, i, station_file);
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
                    if (append_new_address(local_station_file, get_highest_id(local_station_file)+1, address) != 0) {
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
                num_of_addresses = add_stations(address, local_station_file);
                if (num_of_addresses <= 0) {
                    error_message_popup(diolouge, "There was a error parsing the file provided\r\nor the file was not valid");
                }
            }

            if (num_of_addresses <= 0) {
                error_message_popup(diolouge, "There was a issue somewhere");
            }

            if (append_new_station(local_station_file, get_highest_id(local_station_file)+1, name_value, thumbnail_path, num_of_addresses) != 0) {
                error_message_popup(diolouge, "There was a error adding the station");
            }

            GtkWidget* fixed;

            GList* list = gtk_container_get_children(GTK_CONTAINER(data));

            if (GTK_IS_FIXED(list->data)) {
                fixed = GTK_WIDGET(list->data);
            }
            else {
                while((list = g_list_next(list)) != NULL) {
                    if (GTK_IS_FIXED(list->data)) {
                        fixed = GTK_WIDGET(list->data);
                        break;
                    }
                }
            }

            GtkWidget* scrolled;

            GList* list2 = gtk_container_get_children(GTK_CONTAINER(fixed));

            if (GTK_IS_SCROLLED_WINDOW(list2->data)) {
                scrolled = GTK_WIDGET(list2->data);
            }
            else {
                while((list2 = g_list_next(list2)) != NULL) {
                    if (GTK_IS_SCROLLED_WINDOW(list2->data)) {
                        scrolled = GTK_WIDGET(list2->data);
                        break;
                    }
                }
            }

            GtkWidget* view = gtk_bin_get_child(GTK_BIN(scrolled));


            add_station(gtk_bin_get_child(GTK_BIN(view)), name_value, thumbnail_path, get_highest_id(local_station_file), LOCAL);

        break;
        default:

        break;
    }
    gtk_widget_destroy(diolouge);
}

static void event_box_clicked_cb(GtkWidget* widget, gpointer data) {
    int id = cat_station_button_get_id(widget);
    CatStationFile file = cat_station_button_get_station_file(widget);

    most_recent_id = id;
    most_recent_file = file;

    //get the gtk window
    GtkWidget* flow_child = gtk_widget_get_parent(widget);
    GtkWidget* flow = gtk_widget_get_parent(flow_child);
    GtkWidget* view = gtk_widget_get_parent(flow);
    GtkWidget* scroll = gtk_widget_get_parent(view);
    GtkWidget* fixed = gtk_widget_get_parent(scroll);
    GtkWidget* window = gtk_widget_get_parent(fixed);

    if (start_playing(id, file) != 0) {
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

    GtkDialogFlags flags = GTK_DIALOG_MODAL;

    dialog = gtk_dialog_new_with_buttons("ERROR", GTK_WINDOW(parrent), flags, "Ok", GTK_RESPONSE_NONE, NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    label = gtk_label_new(error_message);

    gtk_container_add(GTK_CONTAINER(content_area), label);

    gtk_widget_show_all(dialog);

    (void) gtk_dialog_run(GTK_DIALOG(dialog));
    //when this is done just quit and destroy the dialog and the parent

    gtk_widget_destroy(dialog);

    //gtk_widget_destroy(parrent);

}

int start_playing(int station_id, CatStationFile file) {
    char* file_name = stations_file;
    switch (file) {
        case SYSTEM:
            file_name = stations_file;
            break;
        case LOCAL:
            file_name = local_station_file;
            break;
        default:
            fprintf(stderr, "Something went wrong\r\n");
            break;
    }

    char* address = get_address(file_name, station_id);

    char* thumbnail;

    thumbnail = read_station_thumbnail(file_name, station_id);

    char* name;

    name = read_station_name(file_name, station_id);

    gst_element_set_state(pipeline, GST_STATE_READY);

    if (is_valid_url(address) == 0 && contains_a_pls(address) == 0) {
            char* true_address;
            true_address = get_address_from_pls_over_net(address);

            if (is_valid_url(true_address) != 0) return -1;

            g_object_set(pipeline, "uri", true_address, NULL);

            free(true_address);

    }
    else if (is_valid_url(address) == 0) {

        g_object_set(pipeline, "uri", address, NULL);

    }
    else return -1;

    most_recent_reroll = 1;

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    gtk_widget_show(stop_button);
    gtk_widget_show(pause_button);
    gtk_widget_hide(play_button);
    change_station_playing_image(thumbnail);
    gtk_label_set_text(GTK_LABEL(station_name_label), name);

    free(address);
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
    char* file_name = stations_file;
    switch (most_recent_file) {
        case SYSTEM:
            file_name = stations_file;
            break;
        case LOCAL:
            file_name = local_station_file;
            break;
        default:
            fprintf(stderr, "Something went wrong\r\n");
            break;
    }

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    char* thumbnail;

    thumbnail = read_station_thumbnail(file_name, most_recent_id);

    char* name;

    name = read_station_name(file_name, most_recent_id);

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

void change_station_playing_image(char* thumbnail) {
    gtk_image_set_from_icon_name(GTK_IMAGE(station_image), "audio-x-generic", GTK_ICON_SIZE_BUTTON);
    img_and_dims* info = malloc((sizeof(int)*2)+8);

    info->image = station_image;
    info->x = 50;
    info->y = 50;

    GFile* fp = g_file_new_for_uri(thumbnail);
    g_file_read_async(fp, 1, NULL, file_read_cb, info);
}

GtkWidget* make_image_from_resource(const char* address, int x, int y) {
    GtkWidget* image;

    image = gtk_image_new_from_icon_name("audio-x-generic", GTK_ICON_SIZE_BUTTON);

    img_and_dims* info = malloc((sizeof(int)*2)+8);

    info->image = image;
    info->x = x;
    info->y = y;

    GFile* fp = g_file_new_for_uri(address);
    g_file_read_async(fp, 1, NULL, file_read_cb, info);

    return image;
}

static void image_loaded_cb(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GdkPixbuf* pixbuf;

    pixbuf = gdk_pixbuf_new_from_stream_finish(res, NULL);

    img_and_dims* info = (img_and_dims*) user_data;

    if (!pixbuf) {
        gtk_image_set_from_icon_name(GTK_IMAGE(info->image), "audio-x-generic", GTK_ICON_SIZE_BUTTON);
    }
    else {
        gtk_image_set_from_pixbuf(GTK_IMAGE(info->image), pixbuf);
    }
    
    g_object_unref(pixbuf);
    free(user_data);
}

static void file_read_cb(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GInputStream* stream = G_INPUT_STREAM(g_file_read_finish(G_FILE(source_object), res, NULL));

    img_and_dims* info = (img_and_dims*) user_data;

    gdk_pixbuf_new_from_stream_at_scale_async(stream, info->x, info->y, FALSE, NULL, image_loaded_cb, user_data);

    g_object_unref(stream);
}

void localDB(void) {
    char* home = getenv("HOME");

    if (home == NULL) {
        uid_t uid = getuid();
        struct passwd* pw = getpwuid(uid);
        home = pw->pw_dir;
    }

    //who cares it makes more sense to use sizeof(char)
    local_station_file = calloc(strlen(home)+30, 1);



    strcat(local_station_file, home);
    strcat(local_station_file, "/.config/rajio/local_stations");

    //printf("%s\r\n", local_station_file);

    if (local_exsits(local_station_file)==1) {
        char* work_around = calloc(strlen(home)+8, 1);
        strcat(work_around, home);
        strcat(work_around, "/.config");


        mkdir(work_around, 0777);
        char* work_around2 = calloc(strlen(home)+15, 1);

        strcat(work_around2, work_around);
        strcat(work_around2, "/rajio");

        mkdir(work_around2, 0777);

        free(work_around2);
        //free(work_around);

        if (makeDB(local_station_file)==1) {
            fprintf(stderr, "There was a error making: %s\r\n", local_station_file);
        }  
    }
}

void eos_changer(void) {
    char* file_name = stations_file;
    switch (most_recent_file) {
        case SYSTEM:
            file_name = stations_file;
            break;
        case LOCAL:
            file_name = local_station_file;
            break;
        default:
            fprintf(stderr, "Something went wrong\r\n");
            break;
    }

    if (get_num_of_addresses(file_name, most_recent_id) > 1) {
        if (most_recent_reroll == get_num_of_addresses(file_name, most_recent_id)) {

            most_recent_reroll = 1;

            if (most_recent_id == get_highest_id(file_name)) {
                if (most_recent_file == SYSTEM) {
                    most_recent_file = LOCAL;
                }
                else if (most_recent_file == LOCAL) {
                    most_recent_file = SYSTEM;
                }
                most_recent_id = 1;
            }
            else {
                most_recent_id++;
            }
        }
        else {
            most_recent_reroll++;
        }

    }
    else {
        if (most_recent_id == get_highest_id(file_name)) {
            if (most_recent_file == SYSTEM) {
                most_recent_file = LOCAL;
            }
            else if (most_recent_file == LOCAL) {
                most_recent_file = SYSTEM;
            }
            most_recent_id = 1;
        }
        else {
            most_recent_id++;
        }
    }

    if (start_playing_with_reroll(most_recent_id, most_recent_reroll, most_recent_file) != 0) {
        fprintf(stderr, "There was a error\r\n");
    }
}

int start_playing_with_reroll(int station_id, int reroll, CatStationFile file) {
    char* file_name = stations_file;
    switch (file) {
        case SYSTEM:
            file_name = stations_file;
            break;
        case LOCAL:
            file_name = local_station_file;
            break;
        default:
            fprintf(stderr, "Something went wrong\r\n");
            break;
    }

    char* address = address_reroll(file_name, station_id, reroll);

    most_recent_reroll = reroll;

    char* thumbnail;

    thumbnail = read_station_thumbnail(file_name, station_id);

    char* name;

    name = read_station_name(file_name, station_id);

    gst_element_set_state(pipeline, GST_STATE_READY);

    if (is_valid_url(address) == 0 && contains_a_pls(address) == 0) {
            char* true_address;
            true_address = get_address_from_pls_over_net(address);

            if (is_valid_url(true_address) != 0) return -1;

            g_object_set(pipeline, "uri", true_address, NULL);

            free(true_address);

    }
    else if (is_valid_url(address) == 0) {

        g_object_set(pipeline, "uri", address, NULL);

    }
    else return -1;

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    gtk_widget_show(stop_button);
    gtk_widget_show(pause_button);
    gtk_widget_hide(play_button);
    change_station_playing_image(thumbnail);
    gtk_label_set_text(GTK_LABEL(station_name_label), name);

    free(address);
    free(thumbnail);
    free(name);

    return 0;
}
