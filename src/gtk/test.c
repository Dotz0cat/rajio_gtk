#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop
#include <stdlib.h>
#include <stdio.h>

#include "station_button.h"

static void destroy(GtkWidget *widget, gpointer data);
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

int main(int argc, char** argv) {
	gtk_init(&argc, &argv);

	GtkWidget* window;
	GtkWidget* box;
	GtkWidget* station_button;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "test");

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	gtk_container_add(GTK_CONTAINER(window), box);

	station_button = cat_station_button_new();

	gtk_container_add(GTK_CONTAINER(box), station_button);

	GtkWidget* label = gtk_label_new("I am a label\r\nA child of this button");

	gtk_container_add(GTK_CONTAINER(station_button), label);

	cat_station_button_set_id(station_button, 5);
	cat_station_button_set_id(station_button, 0);
	printf("id: %i\r\n", cat_station_button_get_id(station_button));

	g_signal_connect(window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK (destroy), NULL);

    g_signal_connect(GTK_BUTTON(station_button), "clicked", G_CALLBACK(destroy), NULL);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

static void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    return FALSE; 
}
