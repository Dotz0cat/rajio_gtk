/*
Copyright 2021 Dotz0cat

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

#include "callbacks.h"
#include "gui.h"
#include "parser.h"
#include "station_reader.h"
#include "helpers.h"
#include "rajio_app.h"

void add_button_callback(GtkButton* button, RajioApp* app) {
    g_signal_connect(GTK_BUTTON(button), "button_press_event", G_CALLBACK(event_box_clicked_cb), app);
}

static void event_box_clicked_cb(GtkWidget* widget, gpointer data) {
    int id = cat_station_button_get_id(CAT_STATION_BUTTON(widget));
    CatStationFile file = cat_station_button_get_station_file(CAT_STATION_BUTTON(widget));

    rajio_app_set_most_recent_id(RAJIO_APP(data), id);
    rajio_app_set_most_recent_file(RAJIO_APP(data), file);

    if (start_playing(id, file, RAJIO_APP(data)) != 0) {
        error_message_popup(rajio_app_get_gui(RAJIO_APP(data))->window, "There was a error somewhere");
        return;
    }

    return;
}

void add_other_button_callbacks(UIWidgets* UI, RajioApp* app) {
    g_signal_connect(UI->station_add, "clicked", G_CALLBACK(button_clicked_cb), app);
    g_signal_connect(UI->play, "clicked", G_CALLBACK(play_button_clicked_cb), app);
    g_signal_connect(UI->stop, "clicked", G_CALLBACK(stop_button_clicked_cb), app);
    g_signal_connect(UI->pause, "clicked", G_CALLBACK(pause_button_clicked_cb), app);
}

static void button_clicked_cb(GtkWidget* widget, gpointer data) {

    DialogWidgets* DIALOG = build_dialog(rajio_app_get_gui(RAJIO_APP(data))->window);

    char* name_value;
    char* thumbnail_path;
    int num_of_addresses;

    g_signal_connect(DIALOG->thumbnail_chooser, "clicked", G_CALLBACK(file_chooser_thumbnail_clicked_cb), DIALOG);
    g_signal_connect(DIALOG->address_file_chooser, "clicked", G_CALLBACK(file_chooser_address_clicked_cb), DIALOG);

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
                    if (append_new_address(rajio_app_get_local_file(RAJIO_APP(data)), get_highest_id(rajio_app_get_local_file(RAJIO_APP(data)))+1, address) != 0) {
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
                num_of_addresses = add_stations(address, rajio_app_get_local_file(RAJIO_APP(data)));
                if (num_of_addresses <= 0) {
                    error_message_popup(DIALOG->dialog, "There was a error parsing the file provided\r\nor the file was not valid");
                    break;
                }
                g_free(address);
            }

            if (num_of_addresses <= 0) {
                error_message_popup(DIALOG->dialog, "There was a issue somewhere");
            }

            if (append_new_station(rajio_app_get_local_file(RAJIO_APP(data)), get_highest_id(rajio_app_get_local_file(RAJIO_APP(data)))+1, name_value, thumbnail_path, num_of_addresses) != 0) {
                error_message_popup(DIALOG->dialog, "There was a error adding the station");
            }

            add_station(rajio_app_get_gui(RAJIO_APP(data))->flow, name_value, thumbnail_path, get_highest_id(rajio_app_get_local_file(RAJIO_APP(data))), LOCAL, RAJIO_APP(data));

            g_free(name_value);
            free(thumbnail_path);
        break;
        default:

        break;
    }
    
    gtk_widget_destroy(DIALOG->dialog);

    free(DIALOG);
}

//this is for the thumbnail
static void file_chooser_thumbnail_clicked_cb(GtkWidget* widget, gpointer dialog) {
    DialogWidgets* DIALOG = (DialogWidgets*) dialog;

    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    GtkWidget* chooser;

    chooser = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(DIALOG->dialog), action, "Cancel", GTK_RESPONSE_CANCEL, "Ok", GTK_RESPONSE_ACCEPT, NULL);

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

    chooser = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(DIALOG->dialog), action, "Cancel", GTK_RESPONSE_CANCEL, "Ok", GTK_RESPONSE_ACCEPT, NULL);

    int response = gtk_dialog_run(GTK_DIALOG(chooser));

    if (response == GTK_RESPONSE_ACCEPT) {
        char* filename;

        filename = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(chooser));

        gtk_entry_set_text(GTK_ENTRY(DIALOG->address_file_entry), filename);

        g_free(filename);
    }

    gtk_widget_destroy(chooser);
}

static void play_button_clicked_cb(GtkWidget* widget, gpointer data) {
    char* file_name = rajio_app_get_system_file(RAJIO_APP(data));
    switch (rajio_app_get_most_recent_file(RAJIO_APP(data))) {
        case SYSTEM:
            file_name = rajio_app_get_system_file(RAJIO_APP(data));
            break;
        case LOCAL:
            file_name = rajio_app_get_local_file(RAJIO_APP(data));
            break;
        default:
            fprintf(stderr, "Something went wrong\r\n");
            break;
    }

    gst_element_set_state(rajio_app_get_pipeline(RAJIO_APP(data)), GST_STATE_PLAYING);

    char* thumbnail;

    thumbnail = read_station_thumbnail(file_name, rajio_app_get_most_recent_id(RAJIO_APP(data)));

    char* name;

    name = read_station_name(file_name, rajio_app_get_most_recent_id(RAJIO_APP(data)));

    UIWidgets* UI = rajio_app_get_gui(RAJIO_APP(data));

    gtk_widget_show(UI->stop);
    gtk_widget_show(UI->pause);
    gtk_widget_hide(UI->play);
    change_station_playing_image(thumbnail, UI->station_image);
    gtk_label_set_text(GTK_LABEL(UI->station_label), name);

    free(thumbnail);
    free(name);
}

static void stop_button_clicked_cb(GtkWidget* widget, gpointer data) {
    if (stop_playing(RAJIO_APP(data)) != 0) {
        error_message_popup(rajio_app_get_gui(RAJIO_APP(data))->window, "There was a error with stoping");
    }
}

static void pause_button_clicked_cb(GtkWidget* widget, gpointer data) {
    gst_element_set_state(rajio_app_get_pipeline(RAJIO_APP(data)), GST_STATE_PAUSED);

    UIWidgets* UI = rajio_app_get_gui(RAJIO_APP(data));

    gtk_label_set_text(GTK_LABEL(UI->station_label), "Paused");

    gtk_widget_show(UI->play);
    gtk_widget_show(UI->stop);
    gtk_widget_hide(UI->pause);
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

    gtk_widget_destroy(dialog);
}

void async_read_wrapper(GFile* file, int io_priority, GCancellable* cancellable, gpointer data) {
    g_file_read_async(file, io_priority, cancellable, file_read_cb, data);
}

static void file_read_cb(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GInputStream* stream = G_INPUT_STREAM(g_file_read_finish(G_FILE(source_object), res, NULL));

    img_and_dims* info = (img_and_dims*) user_data;

    gdk_pixbuf_new_from_stream_at_scale_async(stream, info->x, info->y, FALSE, NULL, image_loaded_cb, user_data);

    g_object_unref(stream);
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
