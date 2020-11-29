#include "rajio.h"

typedef struct _WindowInfo WindowInfo;

struct _WindowInfo {
	GtkWidget* window;
	GtkWidget* flow;
	GtkWidget* station_image;
	GtkWidget* station_label;
	GtkWidget* station_add;
	GtkWidget* pause;
	GtkWidget* play;
	GtkWidget* stop;
};

WindowInfo* build_gui(void);
static void destroy(GtkWidget *widget, gpointer data);
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

int main(int argc, char** argv) {
	WindowInfo* info = build_gui();

	gtk_window_set_title(GTK_WINDOW(info->window), "rajio");

	g_signal_connect(info->window, "delete-event", G_CALLBACK (delete_event), NULL);
    g_signal_connect(info->window, "destroy", G_CALLBACK (destroy), NULL);

	gtk_widget_show_all(info->window);

	gtk_main();
}

WindowInfo* build_gui(void) {
	WindowInfo* win_info = malloc(sizeof(WindowInfo));

	win_info->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	GtkWidget* main_box;
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	GtkWidget* scrolled = gtk_scrolled_window_new(NULL, NULL);

	gtk_box_pack_start(GTK_BOX(main_box), scrolled, FALSE, FALSE, 0);

	win_info->flow = gtk_flow_box_new();

	gtk_container_add(GTK_CONTAINER(scrolled), win_info->flow);

	GtkWidget* second_box;
	second_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	gtk_box_pack_end(GTK_BOX(main_box), second_box, FALSE, FALSE, 0);

	win_info->station_image = gtk_image_new_from_icon_name("audio-x-generic", GTK_ICON_SIZE_BUTTON);

	gtk_box_pack_start(GTK_BOX(second_box), win_info->station_image, FALSE, FALSE, 0);

	win_info->station_label = gtk_label_new("No Station Playing");

	gtk_box_pack_start(GTK_BOX(second_box), win_info->station_label, FALSE, FALSE, 0);
	gtk_box_set_center_widget(GTK_BOX(second_box), win_info->station_label);

	win_info->station_add = gtk_button_new_from_icon_name("value-increase-symbolic", GTK_ICON_SIZE_BUTTON);
	win_info->pause = gtk_button_new_from_icon_name("media-playback-pause-symbolic", GTK_ICON_SIZE_BUTTON);
	win_info->play = gtk_button_new_from_icon_name("media-playback-start-symbolic", GTK_ICON_SIZE_BUTTON);
	win_info->stop = gtk_button_new_from_icon_name("media-playback-stop-symbolic", GTK_ICON_SIZE_BUTTON);

	gtk_box_pack_end(GTK_BOX(second_box), win_info->stop, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(second_box), win_info->play, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(second_box), win_info->pause, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(second_box), win_info->station_add, FALSE, FALSE, 0);

	return win_info;
}

static void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    return FALSE; 
}
