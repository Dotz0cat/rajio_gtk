#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop
#include <stdlib.h>
#include <stdio.h>

#include "station_button.h"



int main(int argc, char** argv) {
	gtk_init(&argc, &argv);

	GtkWidget* window;
	GtkWidget* box;
	GtkWidget* station_button;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "test");

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	gtk_container_add(GTK_CONTAINER(window), box);

	station_button = gtk_station_button_new();

	gtk_container_add(GTK_CONTAINER(box), station_button);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
