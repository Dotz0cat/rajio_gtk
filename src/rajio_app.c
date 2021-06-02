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
#include "helpers.h"
#include "callbacks.h"
#include "g-bus.h"
#include "gui.h"

G_DEFINE_TYPE_WITH_CODE(RajioApp, rajio_app, GTK_TYPE_APPLICATION, G_ADD_PRIVATE(RajioApp));

static void rajio_app_init(RajioApp* app) {
    app->priv = rajio_app_get_instance_private(app);
    app->priv->most_recent_id = 1;
    app->priv->most_recent_reroll = 1;
    app->priv->most_recent_file = LOCAL;
    app->priv->system_file = NULL;
    app->priv->local_file = NULL;
}

static void rajio_app_activate(GApplication* app) {
    RAJIO_APP(app)->priv->UI = build_gui(GTK_APPLICATION(app));

    //UIWidgets* UI = RAJIO_APP(app)->priv->UI;

    UIWidgets* UI = rajio_app_get_gui(RAJIO_APP(app));

    int idle = gtk_application_inhibit(GTK_APPLICATION(app), UI->window, GTK_APPLICATION_INHIBIT_IDLE, "Don't want the music to stop");

    if (idle == 0) {
        fprintf(stderr, "Could not set sleep inhibiter.\r\nPrehaps your DE does not support it\r\n");
    }

    gtk_window_set_default_size(GTK_WINDOW(UI->window), 1000, 400);

    //add the staion images and names to the flowbox
    station_adder(rajio_app_get_system_file(RAJIO_APP(app)), UI->flow, SYSTEM, RAJIO_APP(app));
    station_adder(rajio_app_get_local_file(RAJIO_APP(app)), UI->flow, LOCAL, RAJIO_APP(app));

    //add a dioluge popup to the add station button

    add_other_button_callbacks(UI, RAJIO_APP(app));

    //gtk_button_set_relief(GTK_BUTTON(station_add), GTK_RELIEF_NONE);

    gtk_widget_show_all(UI->window);
    gtk_widget_hide(UI->play);
    gtk_widget_hide(UI->stop);
    gtk_widget_hide(UI->pause);

}

static void rajio_app_startup(GApplication* app) {

    G_APPLICATION_CLASS(rajio_app_parent_class)->startup(app);

    GstBus* bus;

    RAJIO_APP(app)->priv->pipeline = gst_element_factory_make("playbin", NULL);

    bus = gst_element_get_bus(RAJIO_APP(app)->priv->pipeline);

    //GST_PLAY_FLAG_AUDIO
    //int flags = 0x00000002;

    //g_object_set(RAJIO_APP(app)->priv->pipeline, "flags", flags, NULL);

    gst_util_set_object_arg(RAJIO_APP(app)->priv->pipeline, "flags", "audio");

    set_message_handlers(bus, RAJIO_APP(app));

    localDB(RAJIO_APP(app));
}

static void rajio_app_class_init(RajioAppClass* class) {
    G_APPLICATION_CLASS(class)->activate = rajio_app_activate;
    G_APPLICATION_CLASS(class)->startup = rajio_app_startup;
}

GtkApplication* rajio_app_new(void) {
    return g_object_new(CAT_TYPE_RAJIO_APP, "application_id", "net.cat.rajio", "flags", G_APPLICATION_FLAGS_NONE, NULL);
}

UIWidgets* rajio_app_get_gui(RajioApp* app) {
    return app->priv->UI;
}

gchar* rajio_app_get_local_file(RajioApp* app) {
    return app->priv->local_file;
}

void rajio_app_set_local_file(RajioApp* app, gchar* file) {
    app->priv->local_file = file;
}

gchar* rajio_app_get_system_file(RajioApp* app) {
    return app->priv->system_file;
}

void rajio_app_set_system_file(RajioApp* app, gchar* file) {
    app->priv->system_file = g_strdup(file);
}

gint rajio_app_get_most_recent_id(RajioApp* app) {
    return app->priv->most_recent_id;
}

void rajio_app_set_most_recent_id(RajioApp* app, gint id) {
    app->priv->most_recent_id = id;
}

CatStationFile rajio_app_get_most_recent_file(RajioApp* app) {
    return app->priv->most_recent_file;
}

void rajio_app_set_most_recent_file(RajioApp* app, CatStationFile file) {
    app->priv->most_recent_file = file;
}

GstElement* rajio_app_get_pipeline(RajioApp* app) {
    return app->priv->pipeline;
}

gint rajio_app_get_most_recent_reroll(RajioApp* app) {
    return app->priv->most_recent_reroll;
}

void rajio_app_set_most_recent_reroll(RajioApp* app, gint reroll) {
    app->priv->most_recent_reroll = reroll;
}

void rajio_app_increment_most_recent_reroll(RajioApp* app) {
    app->priv->most_recent_reroll++;
}

void rajio_app_increment_most_recent_id(RajioApp* app) {
    app->priv->most_recent_id++;
}
