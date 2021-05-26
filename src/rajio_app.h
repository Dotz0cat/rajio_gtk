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

#ifndef __CAT_APPLICATION_H__
#define __CAT_APPLICATION_H__

//clang is loud
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

#define CAT_TYPE_APPLICATION             (cat_application_get_type())
#define CAT_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_APPLICATION, CatApplication))




typedef struct _CatApplication CatApplication;
typedef struct _CatApplicationClass CatApplicationClass;
typedef struct _CatApplicationPrivate CatApplicationPrivate;

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

struct _CatApplication {
    GtkApplication parent;

    CatApplicationPrivate* priv;
};

struct _CatApplicationClass {
    GtkApplicationClass parent_class;
};

struct _CatApplicationPrivate {
    UIWidgets* UI;
};

GType cat_application_get_type(void);
GtkApplication* cat_application_new(void);

#endif /* __CAT_APPLICATION_H__ */