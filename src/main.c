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

//clang is loud
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gtk/gtk.h>
#pragma clang diagnostic pop

#include "rajio_app.h"

//marcos
#ifndef stations_file
    #define stations_file "/usr/local/share/rajio/stations"
#endif

int main(int argc, char* argv[]) {

    gst_init(&argc, &argv);

    GtkApplication* app;

    app = rajio_app_new();

    rajio_app_set_system_file(RAJIO_APP(app), stations_file);

    return g_application_run(G_APPLICATION(app), argc, argv);
}
