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

void add_button_callback(GtkButton* button, RajioApp* app);
static void event_box_clicked_cb(GtkWidget* widget, gpointer data);
void add_other_button_callbacks(UIWidgets* UI, RajioApp* app);
static void button_clicked_cb(GtkWidget* widget, gpointer data);
static void file_chooser_thumbnail_clicked_cb(GtkWidget* widget, gpointer dialog);
static void file_chooser_address_clicked_cb(GtkWidget* widget, gpointer dialog);
static void play_button_clicked_cb(GtkWidget* widget, gpointer data);
static void stop_button_clicked_cb(GtkWidget* widget, gpointer data);
static void pause_button_clicked_cb(GtkWidget* widget, gpointer data);
static void error_message_popup(GtkWidget* parrent, char* error_message);
void async_read_wrapper(GFile* file, int io_priority, GCancellable* cancellable, gpointer data);
static void file_read_cb(GObject* source_object, GAsyncResult* res, gpointer user_data);
static void image_loaded_cb(GObject* source_object, GAsyncResult* res, gpointer user_data);
