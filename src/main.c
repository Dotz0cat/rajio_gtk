/*
Copyright 2020-2021 Dotz0cat

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

//clang is loud
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

#include "rajio_app.h"

//marcos
#ifndef stations_file
    #define stations_file "/usr/local/share/rajio/stations"
#endif

//prototypes
/*void add_station(GtkWidget* flowbox, char* station_name, char* image_file, int id, CatStationFile station_file);
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
static void destroy(GtkWidget* widget, gpointer data);
static gboolean delete_event(GtkWidget* widget, GdkEvent *event, gpointer data);
static void button_clicked_cb(GtkWidget* widget, gpointer data);
static void event_box_clicked_cb(GtkWidget* widget, gpointer data);
static void file_chooser_thumbnail_clicked_cb(GtkWidget* widget, gpointer dialog);
static void file_chooser_address_clicked_cb(GtkWidget* widget, gpointer dialog);
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
//extern UIWidgets* build_gui(GtkApplication* app);
extern DialogWidgets* build_dialog(GtkWidget* window);

//external parser prototypes
extern int add_stations(char* file_name, char* sql_file);
extern int is_valid_url(char* url);
extern int contains_a_pls(char* url);
extern char* get_address_from_pls_over_net(char* pls_file);
extern int genaric_regex(const char* string, const char* regex_string);
*/


int main(int argc, char* argv[]) {

    gst_init(&argc, &argv);

    GtkApplication* app;

    app = rajio_app_new();

    char* system_file = strdup(stations_file);

    //sprintf(system_file, "%s", stations_file);

    printf("%s\r\n", system_file);
    printf("%s\r\n", stations_file);

    rajio_app_set_system_file(RAJIO_APP(app), system_file);

    return g_application_run(G_APPLICATION(app), argc, argv);
}

/*void add_station(GtkWidget* flowbox, char* station_name, char* image_file, int id, CatStationFile station_file) {
    GtkWidget* grid;

    grid = gtk_grid_new();

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    //make the GtkImage
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
}*/

/*static void destroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

static gboolean delete_event(GtkWidget* widget, GdkEvent *event, gpointer data) {
    return FALSE; 
}*/

/*static void button_clicked_cb(GtkWidget* widget, gpointer data) {

    DialogWidgets* DIALOG = build_dialog(UI->window);

    char* name_value;
    char* thumbnail_path;
    int num_of_addresses;

    g_signal_connect(DIALOG->thumbnail_chooser, "clicked", G_CALLBACK(file_chooser_thumbnail_clicked_cb), (gpointer) DIALOG);
    g_signal_connect(DIALOG->address_file_chooser, "clicked", G_CALLBACK(file_chooser_address_clicked_cb), (gpointer) DIALOG);

    //show gtk stuff and enter a loop
    gtk_widget_show_all(DIALOG->dialog);

    int response = gtk_dialog_run(GTK_DIALOG(DIALOG->dialog));

    switch (response) {
        case(GTK_RESPONSE_ACCEPT):
            //get info and make call to append_new_station() and append_new_address
            name_value = gtk_editable_get_chars(GTK_EDITABLE(DIALOG->name_entry), 0, -1);
            if (strcmp(name_value, "") == 0) {
                error_message_popup(DIALOG->dialog, "Please enter a valid name");
                break;
            }

            char* thumbnail_holder;

            thumbnail_holder = gtk_editable_get_chars(GTK_EDITABLE(DIALOG->thumbnail_entry), 0, -1);
            if (strcmp(thumbnail_holder, "") == 0) {
                error_message_popup(DIALOG->dialog, "Please enter a valid thumbnail path");
                break;
            }
            
            if (genaric_regex(thumbnail_holder, "^\\(http\\|https\\|file\\|ftp\\):\\/\\/") != 0) {
                //assume it is a file
                thumbnail_path = calloc(strlen(thumbnail_holder)+8, 1);
                strcat(thumbnail_path, "file://");
                strcat(thumbnail_path, thumbnail_holder);
            }
            else {
                thumbnail_path = calloc(strlen(thumbnail_holder), 1);
                strcpy(thumbnail_path, thumbnail_holder);
            }
            g_free(thumbnail_holder);

            num_of_addresses = 0;

            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(DIALOG->radio_enter_manually))) {
                //use manual button is toggled
                char* address = gtk_editable_get_chars(GTK_EDITABLE(DIALOG->address_manual_entry), 0, -1);

                if (is_valid_url(address) == 0) {
                    num_of_addresses = 1;
                    if (append_new_address(local_station_file, get_highest_id(local_station_file)+1, address) != 0) {
                        fprintf(stderr, "There was a error adding a address to the table\r\n");
                    }
                }
                else {
                    error_message_popup(DIALOG->dialog, "The address entered was not valid");
                    break;
                }
                g_free(address);
            }
            else {
                //the use file button is toggled
                char* address = gtk_editable_get_chars(GTK_EDITABLE(DIALOG->address_file_entry), 0, -1);
                num_of_addresses = add_stations(address, local_station_file);
                if (num_of_addresses <= 0) {
                    error_message_popup(DIALOG->dialog, "There was a error parsing the file provided\r\nor the file was not valid");
                    break;
                }
                g_free(address);
            }

            if (num_of_addresses <= 0) {
                error_message_popup(DIALOG->dialog, "There was a issue somewhere");
            }

            if (append_new_station(local_station_file, get_highest_id(local_station_file)+1, name_value, thumbnail_path, num_of_addresses) != 0) {
                error_message_popup(DIALOG->dialog, "There was a error adding the station");
            }

            add_station(UI->flow, name_value, thumbnail_path, get_highest_id(local_station_file), LOCAL);

            g_free(name_value);
            free(thumbnail_path);
        break;
        default:

        break;
    }
    
    gtk_widget_destroy(DIALOG->dialog);

    free(DIALOG);
}*/

/*static void event_box_clicked_cb(GtkWidget* widget, gpointer data) {
    int id = cat_station_button_get_id(widget);
    CatStationFile file = cat_station_button_get_station_file(widget);

    most_recent_id = id;
    most_recent_file = file;

    if (start_playing(id, file) != 0) {
        error_message_popup(UI->window, "There was a error somewhere");
        return;
    }

    return;
}*/

//this is for the thumbnail
/*static void file_chooser_thumbnail_clicked_cb(GtkWidget* widget, gpointer dialog) {
    DialogWidgets* DIALOG = (DialogWidgets*) dialog;

    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    GtkWidget* chooser;

    chooser = gtk_file_chooser_dialog_new("Open File", DIALOG->dialog, action, "Cancel", GTK_RESPONSE_CANCEL, "Ok", GTK_RESPONSE_ACCEPT, NULL);

    int response = gtk_dialog_run(GTK_DIALOG(chooser));

    if (response == GTK_RESPONSE_ACCEPT) {
        char* filename;

        filename = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(chooser));

        gtk_entry_set_text(GTK_ENTRY(DIALOG->thumbnail_entry), filename);

        g_free(filename);
    }

    gtk_widget_destroy(chooser);
}

//this is for the address
static void file_chooser_address_clicked_cb(GtkWidget* widget, gpointer dialog) {
    DialogWidgets* DIALOG = (DialogWidgets*) dialog;

    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    GtkWidget* chooser;

    chooser = gtk_file_chooser_dialog_new("Open File", DIALOG->dialog, action, "Cancel", GTK_RESPONSE_CANCEL, "Ok", GTK_RESPONSE_ACCEPT, NULL);

    int response = gtk_dialog_run(GTK_DIALOG(chooser));

    if (response == GTK_RESPONSE_ACCEPT) {
        char* filename;

        filename = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(chooser));

        gtk_entry_set_text(GTK_ENTRY(DIALOG->address_file_entry), filename);

        g_free(filename);
    }

    gtk_widget_destroy(chooser);
}*/

/*static void error_message_popup(GtkWidget* parrent, char* error_message) {
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

    gtk_widget_destroy(dialog);
}*/

/*int start_playing(int station_id, CatStationFile file) {
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

    gtk_widget_show(UI->stop);
    gtk_widget_show(UI->pause);
    gtk_widget_hide(UI->play);
    change_station_playing_image(thumbnail);
    gtk_label_set_text(GTK_LABEL(UI->station_label), name);

    free(address);
    free(thumbnail);
    free(name);

    return 0;
}*/

/*int stop_playing() {

    gst_element_set_state(pipeline, GST_STATE_READY);

    gtk_widget_show(UI->play);
    gtk_widget_hide(UI->stop);
    gtk_widget_hide(UI->pause);
    gtk_image_set_from_icon_name(GTK_IMAGE(UI->station_image), "audio-x-generic", GTK_ICON_SIZE_BUTTON);
    gtk_label_set_text(GTK_LABEL(UI->station_label), "No Station Playing");

    return 0;
}*/

/*static void stop_button_clicked_cb(GtkWidget* widget, gpointer data) {
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

    gtk_widget_show(UI->stop);
    gtk_widget_show(UI->pause);
    gtk_widget_hide(UI->play);
    change_station_playing_image(thumbnail);
    gtk_label_set_text(GTK_LABEL(UI->station_label), name);

    free(thumbnail);
    free(name);
}

static void pause_button_clicked_cb(GtkWidget* widget, gpointer data) {
    gst_element_set_state(pipeline, GST_STATE_PAUSED);

    gtk_label_set_text(GTK_LABEL(UI->station_label), "Paused");

    gtk_widget_show(UI->play);
    gtk_widget_show(UI->stop);
    gtk_widget_hide(UI->pause);
}*/

/*void change_station_playing_image(char* thumbnail) {
    gtk_image_set_from_icon_name(GTK_IMAGE(UI->station_image), "audio-x-generic", GTK_ICON_SIZE_BUTTON);
    img_and_dims* info = malloc((sizeof(int)*2)+8);

    info->image = UI->station_image;
    info->x = 50;
    info->y = 50;

    GFile* fp = g_file_new_for_uri(thumbnail);
    g_file_read_async(fp, 1, NULL, file_read_cb, info);
}*/

/*GtkWidget* make_image_from_resource(const char* address, int x, int y) {
    GtkWidget* image;

    image = gtk_image_new_from_icon_name("audio-x-generic", GTK_ICON_SIZE_BUTTON);

    img_and_dims* info = malloc((sizeof(int)*2)+8);

    info->image = image;
    info->x = x;
    info->y = y;

    GFile* fp = g_file_new_for_uri(address);
    g_file_read_async(fp, 1, NULL, file_read_cb, info);

    return image;
}*/

/*static void image_loaded_cb(GObject* source_object, GAsyncResult* res, gpointer user_data) {
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
}*/

/*static void file_read_cb(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GInputStream* stream = G_INPUT_STREAM(g_file_read_finish(G_FILE(source_object), res, NULL));

    img_and_dims* info = (img_and_dims*) user_data;

    gdk_pixbuf_new_from_stream_at_scale_async(stream, info->x, info->y, FALSE, NULL, image_loaded_cb, user_data);

    g_object_unref(stream);
}*/

/*void localDB(void) {
    char* home = getenv("HOME");

    if (home == NULL) {
        uid_t uid = getuid();
        struct passwd* pw = getpwuid(uid);
        home = pw->pw_dir;
    }

    local_station_file = calloc(strlen(home)+30, 1);

    strcat(local_station_file, home);
    strcat(local_station_file, "/.config/rajio/local_stations");

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

        if (makeDB(local_station_file)==1) {
            fprintf(stderr, "There was a error making: %s\r\n", local_station_file);
        }  
    }
}*/

/*void eos_changer(void) {
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
}*/

/*int start_playing_with_reroll(int station_id, int reroll, CatStationFile file) {
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

    gtk_widget_show(UI->stop);
    gtk_widget_show(UI->pause);
    gtk_widget_hide(UI->play);
    change_station_playing_image(thumbnail);
    gtk_label_set_text(GTK_LABEL(UI->station_label), name);

    free(address);
    free(thumbnail);
    free(name);

    return 0;
}*/
