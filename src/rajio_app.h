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

#ifndef __RAJIO_APP_H__
#define __RAJIO_APP_H__

//clang is loud
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

#define CAT_TYPE_RAJIO_APP              (rajio_app_get_type())
#define RAJIO_APP(obj)                  (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_RAJIO_APP, RajioApp))




typedef struct _RajioApp RajioApp;
typedef struct _RajioAppClass RajioAppClass;
typedef struct _RajioAppPrivate RajioAppPrivate;

typedef struct _UIWidgets UIWidgets;

struct _UIWidgets {
    GtkWidget* window;
    GtkWidget* flow;
    GtkWidget* station_image;
    GtkWidget* station_label;
    GtkWidget* station_add;
    GtkWidget* pause;
    GtkWidget* play;
    GtkWidget* stop;
};

struct _RajioApp {
    GtkApplication parent;

    RajioAppPrivate* priv;
};

struct _RajioAppClass {
    GtkApplicationClass parent_class;
};

struct _RajioAppPrivate {
    UIWidgets* UI;
};

GType rajio_app_get_type(void);
GtkApplication* rajio_app_new(void);

#endif /* __RAJIO_APP_H__ */