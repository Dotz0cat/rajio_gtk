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

#ifndef __CAT_STATION_BUTTON_H__
#define __CAT_STATION_BUTTON_H__

#include <gdk/gdk.h>
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

G_BEGIN_DECLS

#define CAT_TYPE_STATION_BUTTON				(cat_station_button_get_type())
#define CAT_STATION_BUTTON(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_STATION_BUTTON, CatStationButton))
#define CAT_STATION_BUTTON_CLASS(klass) 	(G_TYPE_CHECK_CLASS_CAST((klass), CAT_STATION_BUTTON, CatStationButtonClass))
#define CAT_IS_STATION_BUTTON(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_STATION_BUTTON))
#define CAT_IS_STATION_BUTTON_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), CAT_TYPE_STATION_BUTTON))
#define CAT_STATION_BUTTON_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), CAT_TYPE_STATION_BUTTON, CAT_STATION_BUTTON_CLASS)



typedef struct _CatStationButton CatStationButton;
typedef struct _CatStationButtonClass CatStationButtonClass;
typedef struct _CatStationButtonPrivate CatStationButtonPrivate;

typedef enum _CatStationFile CatStationFile;

enum _CatStationFile {
    SYSTEM,
    LOCAL
};

struct _CatStationButton {
	GtkButton button;

	CatStationButtonPrivate* priv;
	//guint station_id;
	//enum here
	//enum
};

struct _CatStationButtonClass {
	GtkButtonClass parent_class;

  	//void (* clicked)  (GtkButton* button);
};

GtkWidget* cat_station_button_new(void);
guint cat_station_button_get_id(CatStationButton* button);
void cat_station_button_set_id(CatStationButton* button, guint station_id);
CatStationFile cat_station_button_get_station_file(CatStationButton* button);
void cat_station_button_set_station_file(CatStationButton* button, CatStationFile station_file);

G_END_DECLS

#endif /* __CAT_STATION_BUTTON_H__ */