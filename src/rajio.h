#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <glib-2.0/glib.h>
#include <gstreamer-1.0/gst/gst.h>
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

extern int most_recent_id;
extern GstElement* pipeline;

struct img_and_dims {
        GtkWidget* image;
        int x;
        int y;
    };
