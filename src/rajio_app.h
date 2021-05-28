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
#include "rajio.h"

#define CAT_TYPE_RAJIO_APP              (rajio_app_get_type())
#define RAJIO_APP(obj)                  (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_RAJIO_APP, RajioApp))
#define G_POINTER_TO_RAJIO_APP(p)       ( (RajioApp* ) (p))

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

    GstElement* pipeline;

    CatStationFile most_recent_file;
    int most_recent_id;
    int most_recent_reroll;

    char* local_file;
    char* system_file;
};

GType rajio_app_get_type(void);
GtkApplication* rajio_app_new(void);
UIWidgets* rajio_app_get_gui(RajioApp* app);
char* rajio_app_get_local_file(RajioApp* app);
void rajio_app_set_local_file(RajioApp* app, char* file);
char* rajio_app_get_system_file(RajioApp* app);
void rajio_app_set_system_file(RajioApp* app, char* file);
int rajio_app_get_most_recent_id(RajioApp* app);
void rajio_app_set_most_recent_id(RajioApp* app, int id);
CatStationFile rajio_app_get_most_recent_file(RajioApp* app);
void rajio_app_set_most_recent_file(RajioApp* app, CatStationFile file);
GstElement* rajio_app_get_pipeline(RajioApp* app);
int rajio_app_get_most_recent_reroll(RajioApp* app);
void rajio_app_set_most_recent_reroll(RajioApp* app, int reroll);
void rajio_app_increment_most_recent_reroll(RajioApp* app);
void rajio_app_increment_most_recent_id(RajioApp* app);

#endif /* __RAJIO_APP_H__ */