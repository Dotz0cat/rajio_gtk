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

#include <gstreamer-1.0/gst/gst.h>
#include "rajio_app.h"
#include "helpers.h"

void set_message_handlers(GstBus* bus, RajioApp* app);
void error_handler(GstBus* bus, GstMessage* msg, gpointer data);
void warn_handler(GstBus* bus, GstMessage* msg, gpointer data);
void eos_handler(GstBus* bus, GstMessage* msg, gpointer data);
void have_type(GstElement* typefind, guint probability, GstCaps* caps, gpointer user_data);
void deep_element_stuff(GstBin* self, GstBin* sub_bin, GstElement* element, gpointer data);
void element_stuff(GstBin* self, GstElement* element, gpointer data);
