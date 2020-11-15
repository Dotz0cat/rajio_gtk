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

#include "rajio.h"

//prototypes 
int append_new_station(char* file_name, int id, char* name, char* thumbnail, int num_of_addresses);
int append_new_address(char* file_name, int id, char* address);
char* read_station_name(char* file_name, int id);
char* read_station_thumbnail(char* file_name, int id);
int get_highest_id(char* file_name);
char* get_address(char* file_name, int id);
int local_exsits(const char* file_name);
int makeDB(const char* file_name);



int append_new_station(char* file_name, int id, char* name, char* thumbnail, int num_of_addresses) {
	sqlite3* db;
	sqlite3_stmt* stmt;

	int rc = sqlite3_open(file_name, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "cannot open %s\r\n", file_name);

		sqlite3_close(db);

		return -1;
	}

	char* sql = "INSERT INTO Stations (Id, Name, Thumbnail, Num_of_addresses) VALUES (?, ?, ?, ?);";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, id);
		sqlite3_bind_text(stmt, 2, name, -1, NULL);
		sqlite3_bind_text(stmt, 3, thumbnail, -1, NULL);
		sqlite3_bind_int(stmt, 4, num_of_addresses);
	}
	else {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);

		return -1;
	}

	sqlite3_step(stmt);

	sqlite3_finalize(stmt);

	sqlite3_close(db);

	return 0;
}

int append_new_address(char* file_name, int id, char* address) {
	sqlite3* db;

	sqlite3_stmt* stmt;

	int rc = sqlite3_open(file_name, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "cannot open %s\r\n", file_name);

		sqlite3_close(db);

		return -1;
	}

	char* sql = "INSERT INTO Addresses (Id, Address) VALUES (?, ?);";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, id);
		sqlite3_bind_text(stmt, 2, address, -1, NULL);
	}
	else {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);

		return -1;
	}

	sqlite3_step(stmt);

	sqlite3_finalize(stmt);

	sqlite3_close(db);

	return 0;
}

//this is on the heap
char* read_station_name(char* file_name, int id) {
	char* name;

	name = malloc(1024 * sizeof(char));

	sqlite3* db;

	sqlite3_stmt* stmt;

	int rc = sqlite3_open(file_name, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "cannot open %s\r\n", file_name);

		sqlite3_close(db);

	}

	char* sql = "SELECT Name FROM Stations WHERE Id = ?";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, id);
	}
	else {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);

	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_ROW) {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);
	}

	sprintf(name, "%s", sqlite3_column_text(stmt, 0));

	sqlite3_finalize(stmt);

	sqlite3_close(db);

	return name;
}

//this is on the heap
char* read_station_thumbnail(char* file_name, int id) {
	char* thumbnail;

	thumbnail = malloc(1024 * sizeof(char));

	sqlite3* db;

	sqlite3_stmt* stmt;

	int rc = sqlite3_open(file_name, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "cannot open %s\r\n", file_name);

		sqlite3_close(db);

	}

	char* sql = "SELECT Thumbnail FROM Stations WHERE Id = ?";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, id);
	}
	else {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);

	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_ROW) {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);
	}

	sprintf(thumbnail, "%s", sqlite3_column_text(stmt, 0));

	sqlite3_finalize(stmt);

	sqlite3_close(db);

	return thumbnail;
}

int get_highest_id(char* file_name) {
	int highest;

	sqlite3* db;

	sqlite3_stmt* stmt;

	int rc = sqlite3_open(file_name, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "cannot open %s\r\n", file_name);

		sqlite3_close(db);

	}

	char* sql = "SELECT Id FROM Stations ORDER BY Id DESC LIMIT 1;";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc == SQLITE_OK) {
		//sqlite3_bind_int(stmt, 1, id);
	}
	else {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);

	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_ROW) {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);
	}

	highest = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);

	sqlite3_close(db);

	return highest;
}

//this is on the heap
char* get_address(char* file_name, int id) {
	char* address;

	address = malloc(1024 * sizeof(char));

	sqlite3* db;

	sqlite3_stmt* stmt;

	int rc = sqlite3_open(file_name, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "cannot open %s\r\n", file_name);

		sqlite3_close(db);

	}

	char* sql = "SELECT Address FROM Addresses WHERE Id = ? LIMIT 1";

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

	if (rc == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, id);
	}
	else {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);

	}

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_ROW) {
		fprintf(stderr, "error: %s\r\n", sqlite3_errmsg(db));

		sqlite3_close(db);
	}

	sprintf(address, "%s", sqlite3_column_text(stmt, 0));

	sqlite3_finalize(stmt);

	sqlite3_close(db);

	return address;
}

int local_exsits(const char* file_name) {
	if (access(file_name, F_OK)) {
		//return 1 if not exsits
		return 1;
	}
	//return 0 if exsits
	return 0;
}

int makeDB(const char* file_name) {
	sqlite3* db;

	int rc = sqlite3_open(file_name, &db);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQLite3 error: %s\r\n", sqlite3_errmsg(db));
		fprintf(stderr, "SQLITE3 error info: %i\r\n", sqlite3_system_errno(db));

		sqlite3_close(db);

		return 1;
	}

	char* sql = "CREATE TABLE Stations (Id INT, Name TEXT, Thumbnail TEXT, Num_of_addresses INT);"
					"CREATE TABLE Addresses (Id INT, Address TEXT);";

	rc = sqlite3_exec(db, sql, NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		sqlite3_close(db);

		return 1;
	}

	sqlite3_close(db);

	return 0;
}
