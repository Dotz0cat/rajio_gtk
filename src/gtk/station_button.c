/*
Copyright 2020 Dotz0cat

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

static GtkWidgetClass* parent_class = NULL;

static void gtk_station_button_class_init(GtkStationButtonClass* class);
static void gtk_station_button_init(GtkStationButton* station_button);

//static void gtk_station_button_destroy(GObject* obj);

guint gtk_station_button_get_type(void) {
    static guint widget_type = 0;

    if (!widget_type) {
        GTypeInfo widget_info {
            "GtkStationButton",
            sizeof(GtkStationButton),
            sizeof(GtkStationButtonClass),
            (GtkClassInitFunc) gtk_station_button_class_init,
            (GtkObjectInitFunc) gtk_station_button_init,
            (GtkArgSetFunc) NULL,
            (GtkArgGetFunc) NULL
        };

        widget_type = gtk_type_unique(gtk_widget_get_type(), &widget_info);
    }
    return widget_type;
}

static void gtk_station_button_class_init(GtkStationButtonClass* class) {
    GObject* gobject_class;
    GtkWidgetClass* widget_class;
    GtkButtonClass* button_class;

    gobject_class = G_OBJECT_CLASS(class);
    widget_class = (GtkWidgetClass* ) class;
    button_class = (GtkButtonClass* ) class;


}

static void gtk_station_button_init(GtkStationButton* button) {
    GtkWidget* widget;

    widget = GTK_WIDGET(button);

    //inital values

    button->button = NULL;

    button->child = NULL;

    button->station_id = 0;
}

GtkWidget* gtk_station_button_new() {
    return g_object_new(gtk_station_button_get_type(), GTK_TYPE_BUTTON,  NULL);
}