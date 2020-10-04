#include "rajio.h"

//prototypes 
int append_new_station(char* file_name, int id, char* name, char* thumbnail, int num_of_addresses);
int append_new_address(char* file_name, int id, char* address);
char* read_station_name(char* file_name, int id);
char* read_station_thumbnail(char* file_name, int id);
int get_highest_id(char* file_name);
char* get_address(char* file_name, int id);



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
