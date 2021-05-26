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

#include "rajio_app.h"

G_DEFINE_TYPE_WITH_CODE(RajioApp, rajio_app, GTK_TYPE_APPLICATION, G_ADD_PRIVATE(RajioApp));

static void rajio_app_init(RajioApp* app) {
    app->priv = rajio_app_get_instance_private(app);
    app->priv->UI = malloc(sizeof(UIWidgets));
}

/*static void quit_activated(GSimpleAction* action, GVariant* parameter, gpointer app) {
  g_application_quit(G_APPLICATION(app));
}*/

static void rajio_app_activate(GApplication* app) {
    //RAJIO_APP(app)->UI = build_gui(app, app->UI);

    UIWidgets* UI = RAJIO_APP(app)->priv->UI;

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

}

static void rajio_app_class_init(RajioAppClass* class) {
    G_APPLICATION_CLASS(class)->activate = rajio_app_activate;
}

GtkApplication* rajio_app_new(void) {
    return g_object_new(CAT_TYPE_RAJIO_APP, "application_id", "net.cat.rajio", "flags", G_APPLICATION_FLAGS_NONE, NULL);
}

UIWidgets* rajio_app_get_gui(RajioApp* app) {
    return app->priv->UI;
}
