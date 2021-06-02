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

#ifndef __RAJIO_H__
#define __RAJIO_H__


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <glib-2.0/glib.h>
#include <gstreamer-1.0/gst/gst.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

#include "gtk/station_button.h"

typedef struct _img_and_dims img_and_dims;

struct _img_and_dims {
        GtkWidget* image;
        int x;
        int y;
    };

typedef struct _DialogWidgets DialogWidgets;

struct _DialogWidgets {
	GtkWidget* dialog;
	GtkWidget* name_entry;
	GtkWidget* thumbnail_entry;
	GtkWidget* thumbnail_chooser;
	GtkWidget* radio_use_file;
    GtkWidget* radio_enter_manually;
    GtkWidget* address_file_entry;
    GtkWidget* address_file_chooser;
    GtkWidget* address_manual_entry;
};

#endif /* __RAJIO_H__ */