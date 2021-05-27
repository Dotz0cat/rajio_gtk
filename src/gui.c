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

#include "gui.h"
#include "rajio_app.h"

UIWidgets* build_gui(GtkApplication* app, UIWidgets* UI) {
	//UIWidgets* UI = malloc(sizeof(UIWidgets));

	UI->window = gtk_application_window_new(app);

	GtkWidget* main_box;
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	gtk_container_add(GTK_CONTAINER(UI->window), main_box);

	GtkWidget* scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_valign(scrolled, GTK_ALIGN_FILL);

	gtk_box_pack_start(GTK_BOX(main_box), scrolled, TRUE, TRUE, 0);

	UI->flow = gtk_flow_box_new();

	gtk_container_add(GTK_CONTAINER(scrolled), UI->flow);
	gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(UI->flow), GTK_SELECTION_NONE);

	GtkWidget* second_box;
	second_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	gtk_box_pack_end(GTK_BOX(main_box), second_box, FALSE, FALSE, 0);

	UI->station_image = gtk_image_new_from_icon_name("audio-x-generic", GTK_ICON_SIZE_BUTTON);

	gtk_widget_set_margin_start(GTK_WIDGET(UI->station_image), 10);
	gtk_widget_set_margin_bottom(GTK_WIDGET(UI->station_image), 10);
	gtk_widget_set_margin_top(GTK_WIDGET(UI->station_image), 10);

	gtk_box_pack_start(GTK_BOX(second_box), UI->station_image, FALSE, FALSE, 0);

	UI->station_label = gtk_label_new("No Station Playing");

	gtk_box_set_center_widget(GTK_BOX(second_box), UI->station_label);
	gtk_widget_set_halign(UI->station_label, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(UI->station_label, GTK_ALIGN_CENTER);

	UI->station_add = gtk_button_new_from_icon_name("value-increase-symbolic", GTK_ICON_SIZE_BUTTON);
	UI->pause = gtk_button_new_from_icon_name("media-playback-pause-symbolic", GTK_ICON_SIZE_BUTTON);
	UI->play = gtk_button_new_from_icon_name("media-playback-start-symbolic", GTK_ICON_SIZE_BUTTON);
	UI->stop = gtk_button_new_from_icon_name("media-playback-stop-symbolic", GTK_ICON_SIZE_BUTTON);

	gtk_box_pack_end(GTK_BOX(second_box), UI->stop, FALSE, FALSE, 0);
	gtk_widget_set_valign(UI->stop, GTK_ALIGN_CENTER);
	gtk_box_pack_end(GTK_BOX(second_box), UI->play, FALSE, FALSE, 0);
	gtk_widget_set_valign(UI->play, GTK_ALIGN_CENTER);
	gtk_box_pack_end(GTK_BOX(second_box), UI->pause, FALSE, FALSE, 0);
	gtk_widget_set_valign(UI->pause, GTK_ALIGN_CENTER);
	gtk_box_pack_end(GTK_BOX(second_box), UI->station_add, FALSE, FALSE, 0);
	gtk_widget_set_valign(UI->station_add, GTK_ALIGN_CENTER);

	return UI;
}

DialogWidgets* build_dialog(GtkWidget* window) {
	DialogWidgets* UI = malloc(sizeof(DialogWidgets));

    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

    UI->dialog = gtk_dialog_new_with_buttons("Add Station", window, flags, "Ok", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_REJECT, NULL);

    //make a grid and stuff and add stuff to it
    GtkWidget* content_area;

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(UI->dialog));

    //grid
    GtkWidget* grid;

    grid = gtk_grid_new();

    gtk_widget_set_name(grid, "add_station_grid");

    //delectations
    GtkWidget* name;
    GtkWidget* thumbnail;
    GtkWidget* address_label;

    //make for name and add to grid
    name = gtk_label_new("Name");

    gtk_grid_attach(GTK_GRID(grid), name, 2, 0, 1, 1);

    //make name entry box
    UI->name_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), UI->name_entry, 1, 1, 4, 1);

    //make thumbnail label
    thumbnail = gtk_label_new("Thumnail");

    gtk_grid_attach(GTK_GRID(grid), thumbnail, 2, 4, 1, 1);

    //make thumbnail entry
    UI->thumbnail_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), UI->thumbnail_entry, 1, 5, 3, 1);

    //add file chooser button for the thumbnail entry
    //make a file chooser dialog popup

    UI->thumbnail_chooser = gtk_button_new();

    gtk_grid_attach(GTK_GRID(grid), UI->thumbnail_chooser, 4, 5, 1, 1);

    //add a label
    address_label = gtk_label_new("Address");

    gtk_grid_attach(GTK_GRID(grid), address_label, 2, 7, 1, 1);

    //add first radio button
    UI->radio_use_file = gtk_radio_button_new(NULL);

    //make address file entry
    UI->address_file_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), UI->radio_use_file, 0, 8, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), UI->address_file_entry, 1, 8, 3, 1);

    //add the button for file selection
    UI->address_file_chooser = gtk_button_new();

    gtk_grid_attach(GTK_GRID(grid), UI->address_file_chooser, 4, 8, 1, 1);

    //add other radio button
    UI->radio_enter_manually = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(UI->radio_use_file));

    //entry box
    UI->address_manual_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), UI->radio_enter_manually, 0, 9, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), UI->address_manual_entry, 1, 9, 4, 1);
    //add grid to content area
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    return UI;
}
