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

#include "rajio.h"
#include "helpers.h"
#include "station_reader.h"
#include "callbacks.h"

void localDB(RajioApp* app) {
    char* home = getenv("HOME");

    if (home == NULL) {
        uid_t uid = getuid();
        struct passwd* pw = getpwuid(uid);
        home = pw->pw_dir;
    }

    char* local_station_file = calloc(strlen(home)+30, 1);

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

    rajio_app_set_local_file(app, local_station_file);
}

void add_station(GtkWidget* flowbox, char* station_name, char* image_file, int id, CatStationFile station_file) {
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

    add_button_callback(GTK_BUTTON(button));

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

int stop_playing(RajioApp* app) {

    gst_element_set_state(rajio_app_get_pipeline(app), GST_STATE_READY);

    UIWidgets* UI = rajio_app_get_gui(app);

    gtk_widget_show(UI->play);
    gtk_widget_hide(UI->stop);
    gtk_widget_hide(UI->pause);
    gtk_image_set_from_icon_name(GTK_IMAGE(UI->station_image), "audio-x-generic", GTK_ICON_SIZE_BUTTON);
    gtk_label_set_text(GTK_LABEL(UI->station_label), "No Station Playing");

    return 0;
}

int start_playing(int station_id, CatStationFile file, RajioApp* app) {
    char* file_name = rajio_app_get_system_file(app);
    switch (file) {
        case SYSTEM:
            file_name = rajio_app_get_system_file(app);
            break;
        case LOCAL:
            file_name = rajio_app_get_local_file(app);
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

    gst_element_set_state(rajio_app_get_pipeline(app), GST_STATE_READY);

    if (is_valid_url(address) == 0 && contains_a_pls(address) == 0) {
            char* true_address;
            true_address = get_address_from_pls_over_net(address);

            if (is_valid_url(true_address) != 0) return -1;

            g_object_set(rajio_app_get_pipeline(app), "uri", true_address, NULL);

            free(true_address);

    }
    else if (is_valid_url(address) == 0) {

        g_object_set(rajio_app_get_pipeline(app), "uri", address, NULL);

    }
    else return -1;

    rajio_app_set_most_recent_reroll(app, 1);

    gst_element_set_state(rajio_app_get_pipeline(app), GST_STATE_PLAYING);

    UIWidgets* UI = rajio_app_get_gui(app);

    gtk_widget_show(UI->stop);
    gtk_widget_show(UI->pause);
    gtk_widget_hide(UI->play);
    change_station_playing_image(thumbnail, UI->station_image);
    gtk_label_set_text(GTK_LABEL(UI->station_label), name);

    free(address);
    free(thumbnail);
    free(name);

    return 0;
}

void change_station_playing_image(char* thumbnail, GtkWidget* station_image) {
    gtk_image_set_from_icon_name(GTK_IMAGE(station_image), "audio-x-generic", GTK_ICON_SIZE_BUTTON);
    img_and_dims* info = malloc(sizeof(img_and_dims));

    info->image = station_image;
    info->x = 50;
    info->y = 50;

    GFile* fp = g_file_new_for_uri(thumbnail);
    //g_file_read_async(fp, 1, NULL, file_read_cb, info);
    async_read_wrapper(fp, 1, NULL, info);
}

GtkWidget* make_image_from_resource(const char* address, int x, int y) {
    GtkWidget* image;

    image = gtk_image_new_from_icon_name("audio-x-generic", GTK_ICON_SIZE_BUTTON);

    img_and_dims* info = malloc(sizeof(img_and_dims));

    info->image = image;
    info->x = x;
    info->y = y;

    GFile* fp = g_file_new_for_uri(address);
    async_read_wrapper(fp, 1, NULL, info);

    return image;
}

void eos_changer(RajioApp* app) {
    char* file_name = rajio_app_get_system_file(app);
    switch (rajio_app_get_most_recent_file(app)) {
        case SYSTEM:
            file_name = rajio_app_get_system_file(app);
            break;
        case LOCAL:
            file_name = rajio_app_get_local_file(app);
            break;
        default:
            fprintf(stderr, "Something went wrong\r\n");
            break;
    }

    if (get_num_of_addresses(file_name, rajio_app_get_most_recent_id(app)) > 1) {
        if (rajio_app_get_most_recent_reroll(app) == get_num_of_addresses(file_name, most_recent_id)) {

            rajio_app_set_most_recent_reroll(app, 1);

            if (rajio_app_get_most_recent_id(app) == get_highest_id(file_name)) {
                if (rajio_app_get_most_recent_file(app) == SYSTEM) {
                    rajio_app_set_most_recent_file(app, LOCAL);
                }
                else if (rajio_app_get_most_recent_file(app) == LOCAL) {
                    rajio_app_set_most_recent_file(app, SYSTEM);
                }
                rajio_app_set_most_recent_id(app, 1);
            }
            else {
                rajio_app_increment_most_recent_id(app);
            }
        }
        else {
            rajio_app_increment_most_recent_reroll(app);
        }

    }
    else {
        if (rajio_app_get_most_recent_id(app) == get_highest_id(file_name)) {
            if (rajio_app_get_most_recent_file(app) == SYSTEM) {
                rajio_app_set_most_recent_file(app, LOCAL);
            }
            else if (rajio_app_get_most_recent_file(app) == LOCAL) {
                rajio_app_set_most_recent_file(app, SYSTEM);
            }
            rajio_app_set_most_recent_id(app, 1);
        }
        else {
            rajio_app_increment_most_recent_id(app);
        }
    }

    if (start_playing_with_reroll(rajio_app_get_most_recent_id(app), rajio_app_get_most_recent_reroll(app), rajio_app_get_most_recent_file(app), app) != 0) {
        error_message_popup(rajio_app_get_gui(app)->window, "There was a error with rerolling");
    }
}

int start_playing_with_reroll(int station_id, int reroll, CatStationFile file, RajioApp* app) {
    char* file_name = rajio_app_get_system_file(app);
    switch (file) {
        case SYSTEM:
            file_name = rajio_app_get_system_file(app);
            break;
        case LOCAL:
            file_name = rajio_app_get_local_file(app);
            break;
        default:
            fprintf(stderr, "Something went wrong\r\n");
            break;
    }

    char* address = address_reroll(file_name, station_id, reroll);

    rajio_app_set_most_recent_reroll(app, reroll);

    char* thumbnail;

    thumbnail = read_station_thumbnail(file_name, station_id);

    char* name;

    name = read_station_name(file_name, station_id);

    gst_element_set_state(rajio_app_get_pipeline(app), GST_STATE_READY);

    if (is_valid_url(address) == 0 && contains_a_pls(address) == 0) {
            char* true_address;
            true_address = get_address_from_pls_over_net(address);

            if (is_valid_url(true_address) != 0) return -1;

            g_object_set(rajio_app_get_pipeline(app), "uri", true_address, NULL);

            free(true_address);

    }
    else if (is_valid_url(address) == 0) {

        g_object_set(rajio_app_get_pipeline(app), "uri", address, NULL);

    }
    else return -1;

    gst_element_set_state(rajio_app_get_pipeline(app), GST_STATE_PLAYING);

    UIWidgets* UI = rajio_app_get_gui(app);

    gtk_widget_show(UI->stop);
    gtk_widget_show(UI->pause);
    gtk_widget_hide(UI->play);
    change_station_playing_image(thumbnail, UI->station_image);
    gtk_label_set_text(GTK_LABEL(UI->station_label), name);

    free(address);
    free(thumbnail);
    free(name);

    return 0;
}
