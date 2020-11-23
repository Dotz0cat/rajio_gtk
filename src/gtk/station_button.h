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

#ifndef __GTK_STATION_BUTTON_H__
#define __GTK_STATION_BUTTON_H__

#include <gdk/gdk.h>
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

G_BEGIN_DECLS

#define GTK_TYPE_STATION_BUTTON				(gtk_station_button_get_type())
#define GTK_STATION_BUTTON(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_STATION_BUTTON, GtkStationButton))
#define GTK_STATION_BUTTON_CLASS(klass) 	(G_TYPE_CHECK_CLASS_CAST((klass), GTK_STATION_BUTTON, GtkStationButtonClass))
#define GTK_IS_STATION_BUTTON(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_STATION_BUTTON))
#define GTK_IS_STATION_BUTTON_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_STATION_BUTTON))
#define GTK_STATION_BUTTON_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_STATION_BUTTON, GTK_STATION_BUTTON_CLASS)



typedef struct _GtkStationButton GtkStationButton;
typedef struct _GtkStationButtonClass GtkStationButtonClass;

struct _GtkStationButton {
	GtkWidget* button;

	GtkWidget* child;

	guint station_id;
	//enum here
};

struct _GtkStationButtonClass {
	GtkButtonClass parent_class;

	/*void (* pressed)  (GtkButton* button);
  	void (* released) (GtkButton* button);
  	void (* clicked)  (GtkButton* button);
  	void (* enter)    (GtkButton* button);
  	void (* leave)    (GtkButton* button);
  	*/
};

GtkWidget* gtk_station_button_new();
//guint gtk_station_button_get_type(void);

G_END_DECLS

#endif /* __GTK_STATION_BUTTON_H__ */