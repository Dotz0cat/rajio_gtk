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

int add_stations(char* file_name, char* sql_file);
int m3u_parser(char* file_name, char* sql_file);
int pls_parser(char* file_name, char* sql_file);
int is_valid_url(char* url);
int contains_a_pls(char* url);
char* get_address_from_pls_over_net(char* pls_file);
int genaric_regex(const char* string, const char* regex_string);
