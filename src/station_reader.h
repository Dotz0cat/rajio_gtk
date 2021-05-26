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

int append_new_station(char* file_name, int id, char* name, char* thumbnail, int num_of_addresses);
int append_new_address(char* file_name, int id, char* address);
char* read_station_name(char* file_name, int id);
char* read_station_thumbnail(char* file_name, int id);
int get_highest_id(char* file_name);
char* get_address(char* file_name, int id);
int local_exsits(const char* file_name);
int makeDB(const char* file_name);
char* address_reroll(const char* file_name, int station_id, int reroll);
int get_num_of_addresses(const char* file_name, int id);