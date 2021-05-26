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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//clang is loud
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop
#include "station_button.h"

struct _CatStationButtonPrivate {
    guint station_id;

    CatStationFile station_file;
};


static void cat_station_button_class_init(CatStationButtonClass* class);
static void cat_station_button_init(CatStationButton* station_button);
guint cat_station_button_get_id(CatStationButton* button);
void cat_station_button_set_id(CatStationButton* button, guint station_id);
CatStationFile cat_station_button_get_station_file(CatStationButton* button);
void cat_station_button_set_station_file(CatStationButton* button, CatStationFile station_file);

G_DEFINE_TYPE_WITH_CODE(CatStationButton, cat_station_button, GTK_TYPE_BUTTON, G_ADD_PRIVATE(CatStationButton))

static void cat_station_button_class_init(CatStationButtonClass* class) {
    GObject* gobject_class;
    GtkWidgetClass* widget_class;
    GtkButtonClass* button_class;

    gobject_class = G_OBJECT_CLASS(class);
    widget_class = (GtkWidgetClass* ) class;
    button_class = (GtkButtonClass* ) class;
}

static void cat_station_button_init(CatStationButton* button) {

    //inital values
    button->priv = cat_station_button_get_instance_private(button);
    button->priv->station_id = 0;
    button->priv->station_file = LOCAL;
}

GtkWidget* cat_station_button_new(void) {
    return g_object_new(CAT_TYPE_STATION_BUTTON, NULL);
}

guint cat_station_button_get_id(CatStationButton* button) {
    return button->priv->station_id;
}

void cat_station_button_set_id(CatStationButton* button, guint station_id) {
    g_return_if_fail(station_id > 0);
    button->priv->station_id = station_id;
}

CatStationFile cat_station_button_get_station_file(CatStationButton* button) {
    return button->priv->station_file;
}

void cat_station_button_set_station_file(CatStationButton* button, CatStationFile station_file) {
    button->priv->station_file = station_file;
}
