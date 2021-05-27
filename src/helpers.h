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

#include "rajio_app.h"
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

void localDB(RajioApp* app);
void add_station(GtkWidget* flowbox, char* station_name, char* image_file, int id, CatStationFile station_file, RajioApp* app);
void station_adder(char* file_name, GtkWidget* flow, CatStationFile station_file, RajioApp* app);
int stop_playing(RajioApp* app);
int start_playing(int station_id, CatStationFile file, RajioApp* app);
void change_station_playing_image(char* thumbnail, GtkWidget* station_image);
GtkWidget* make_image_from_resource(const char* address, int x, int y);
void eos_changer(RajioApp* app);
int start_playing_with_reroll(int station_id, int reroll, CatStationFile file, RajioApp* app);