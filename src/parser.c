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
#include <string.h>
#include <regex.h>
//clang is loud
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <gio/gio.h>
#pragma clang diagnostic pop

#include "parser.h"
#include "station_reader.h"

//external prototypes
extern int append_new_address(char* file_name, int id, char* address);
extern int get_highest_id(char* file_name);

int add_stations(char* file_name, char* sql_file) {
	char string[512];

	strcpy(string, file_name);

	char* token;

	token = strtok(string, ".");

	//check file endings
	//it checks in case of several '.'s
	//next i hope to make it check for magic stuff
	while (token != NULL) {
		//else if trees fun
		if (strcmp(token, "m3u") == 0) {
			return m3u_parser(file_name, sql_file);
		}
		else if (strcmp(token, "pls") == 0) {
			return pls_parser(file_name, sql_file);
		}
		else if (strcmp(token, "m3u8") == 0) {
			return m3u_parser(file_name, sql_file);
		}


		//rerun token in case this is not the only '.'
		token = strtok(NULL, ".");
	}

	//do magic checking here

	//return 0 casuse it had no addresses
	return 0;
}

int m3u_parser(char* file_name, char* sql_file) {
	FILE* fp = fopen(file_name, "r");

	if (!fp) {
		fprintf(stderr, "cannot open: %s\r\n", file_name);

		return -1;
	}

	int addresses = 0;

	char str[512];
	int id = get_highest_id(sql_file) + 1;

	//regex stuff
	regex_t regex;
	regex_t comment_regex;
	int error_num;
	char err_msg[256];

	//old regex [(http(s)?):\/\/(www\.)?a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*)
	//tried ^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$ 

	error_num = regcomp(&regex, "^\\(http\\|ftp\\|https\\|mms\\):\\/\\/\\(www\\.\\)\\?[a-z0-9A-z\\.:]\\{2,256\\}", REG_ICASE);
	if (error_num) {
		regerror(error_num, &regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		fclose(fp);
		return -1;
	}

	error_num = regcomp(&comment_regex, "^#", 0);
	if (error_num) {
		regerror(error_num, &comment_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		fclose(fp);
		return -1;
	}
	
	int comment;

	while (fgets(str, (int) sizeof(str), fp) != NULL) {
		comment = 0;

		error_num = regexec(&comment_regex, str, 0, NULL, 0);
		if(!error_num) {
			comment = 1;
		}
			

		if (comment == 0) {
			error_num = regexec(&regex, str, 0, NULL, 0);
			printf("%s\r\n", str);
			if(!error_num) {
				addresses++;
				if (append_new_address(sql_file, id, str) != 0) {
					fprintf(stderr, "there was a error appending %s to sql file\r\n", str);
					regfree(&regex);
					regfree(&comment_regex);
					fclose(fp);
					return -1;
				}
			}
			/*else {
				regerror(error_num, &comment_regex, err_msg, sizeof(err_msg));
				fprintf(stderr, "error doing regex: %s\n", err_msg);
			}*/
		}
	}

	regfree(&regex);
	regfree(&comment_regex);

	fclose(fp);

	return addresses;
}

int pls_parser(char* file_name, char* sql_file) {
	FILE* fp = fopen(file_name, "r");

	if (!fp) {
		fprintf(stderr, "cannot open: %s\r\n", file_name);

		return -1;
	}

	int addresses = 0;

	//it just looks better to me
	char str[512];
	int id = get_highest_id(sql_file) + 1;

	//regex stuff
	regex_t regex;
	regex_t file_regex;
	regex_t header_regex;
	regex_t http;
	regex_t https;
	regex_t ftp;
	regex_t mms;
	int error_num;
	char err_msg[256];

	error_num = regcomp(&regex, "\\(http\\|ftp\\|https\\|mms\\):\\/\\/\\(www\\.\\)\\?[a-z0-9A-z\\.:]\\{2,256\\}", REG_ICASE);
	if (error_num) {
		regerror(error_num, &regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		fclose(fp);
		return -1;
	}

	error_num = regcomp(&file_regex, "^file[0-9]\\{1,4\\}=", REG_ICASE);
	if (error_num) {
		regerror(error_num, &file_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		fclose(fp);
		return -1;
	}

	error_num = regcomp(&header_regex, "\\[playlist\\]", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		fclose(fp);
		return -1;
	}

	error_num = regcomp(&http, "http:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		fclose(fp);
		return -1;
	}

	error_num = regcomp(&https, "https:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		regfree(&http);
		fclose(fp);
		return -1;
	}

	error_num = regcomp(&ftp, "ftp:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		regfree(&http);
		regfree(&https);
		fclose(fp);
		return -1;
	}

	error_num = regcomp(&mms, "mms:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		regfree(&http);
		regfree(&https);
		regfree(&ftp);
		fclose(fp);
		return -1;
	}

	int header_found = 0;

	fgets(str, (int) sizeof(str), fp);

	error_num = regexec(&header_regex, str, 0, NULL, 0);
	if (!error_num) {
		header_found = 1;
	}

	while (fgets(str, (int) sizeof(str), fp) != NULL) {
		if (header_found == 1) {
			error_num = regexec(&file_regex, str, 0, NULL, 0);
			if (!error_num) {
				error_num = regexec(&regex, str, 0, NULL, 0);
				if (!error_num) {
					
					if (regexec(&http, str, 0, NULL, 0) == 0) {
						char* string;
						string = strstr(str, "http");
						addresses++;
						if (append_new_address(sql_file, id, string) != 0) {
							fprintf(stderr, "there was a error appending %s to sql file\r\n", str);
							regfree(&regex);
							regfree(&file_regex);
							regfree(&header_regex);
							regfree(&http);
							regfree(&https);
							regfree(&ftp);
							regfree(&mms);
							fclose(fp);
							return -1;
						}
					}
					else if (regexec(&https, str, 0, NULL, 0) == 0) {
						char* string;
						string = strstr(str, "https");
						addresses++;
						if (append_new_address(sql_file, id, string) != 0) {
							fprintf(stderr, "there was a error appending %s to sql file\r\n", str);
							regfree(&regex);
							regfree(&file_regex);
							regfree(&header_regex);
							regfree(&http);
							regfree(&https);
							regfree(&ftp);
							regfree(&mms);
							fclose(fp);
							return -1;
						}
					}
					else if (regexec(&ftp, str, 0, NULL, 0) == 0) {
						char* string;
						string = strstr(str, "ftp");
						addresses++;
						if (append_new_address(sql_file, id, string) != 0) {
							fprintf(stderr, "there was a error appending %s to sql file\r\n", str);
							regfree(&regex);
							regfree(&file_regex);
							regfree(&header_regex);
							regfree(&http);
							regfree(&https);
							regfree(&ftp);
							regfree(&mms);
							fclose(fp);
							return -1;
						}
					}
					else if (regexec(&mms, str, 0, NULL, 0) == 0) {
						char* string;
						string = strstr(str, "mms");
						addresses++;
						if (append_new_address(sql_file, id, string) != 0) {
							fprintf(stderr, "there was a error appending %s to sql file\r\n", str);
							regfree(&regex);
							regfree(&file_regex);
							regfree(&header_regex);
							regfree(&http);
							regfree(&https);
							regfree(&ftp);
							regfree(&mms);
							fclose(fp);
							return -1;
						}
					}
				}
			}
		}
	}

	regfree(&regex);
	regfree(&file_regex);
	regfree(&header_regex);
	regfree(&http);
	regfree(&https);
	regfree(&ftp);
	regfree(&mms);

	fclose(fp);

	return addresses;
}

//returns 0 if it is a valid url
int is_valid_url(char* url) {
	regex_t regex;
	int error_num;
	char err_msg[256];

	error_num = regcomp(&regex, "^\\(http\\|ftp\\|https\\|mms\\):\\/\\/\\(www\\.\\)\\?[a-z0-9A-z\\.:]\\{2,256\\}", REG_ICASE);
	if (error_num) {
		regerror(error_num, &regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\r\n", err_msg);
		return -1;
	}

	error_num = regexec(&regex, url, 0, NULL, 0);
	if (!error_num) {
		regfree(&regex);
		return 0;
	}

	regfree(&regex);

	return 1;
}

int contains_a_pls(char* url) {
	regex_t regex;
	int error_num;
	char err_msg[256];

	error_num = regcomp(&regex, "\\.pls", REG_ICASE);
	if (error_num) {
		regerror(error_num, &regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\r\n", err_msg);
		return -1;
	}

	error_num = regexec(&regex, url, 0, NULL, 0);
	if (!error_num) {
		regfree(&regex);
		return 0;
	}

	regfree(&regex);

	return 1;
}

//this will be on the heap
char* get_address_from_pls_over_net(char* pls_file) {
	GFile* fp = g_file_new_for_uri(pls_file);
	GFileInputStream* stream = g_file_read(fp, NULL, NULL);

	if (!stream) {
		g_object_unref(fp);
		return "";
	}

	GDataInputStream* data_stream = g_data_input_stream_new(G_INPUT_STREAM(stream));

	regex_t regex;
	regex_t file_regex;
	regex_t header_regex;
	regex_t http;
	regex_t https;
	regex_t ftp;
	regex_t mms;
	int error_num;
	char err_msg[256];

	char str[256] = "";

	int tmp;

	char* temp;

	char number[256];

	temp = strstr(pls_file, "id=");

	strcat(str, "^file");
	sscanf(temp, "%*[^1234567890]%i", &tmp);
	sprintf(number, "%i", tmp);
	strcat(str, number);
	strcat(str, "=");

	//printf("regex: %s\r\n", str);

	error_num = regcomp(&regex, "\\(http\\|ftp\\|http\\):\\/\\/\\(www\\.\\)\\?[a-z0-9A-z\\.:]\\{2,256\\}", REG_ICASE);
	if (error_num) {
		regerror(error_num, &regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		g_object_unref(stream);
		g_object_unref(fp);

		return "";
	}

	error_num = regcomp(&file_regex, str, REG_ICASE);
	if (error_num) {
		regerror(error_num, &file_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		g_object_unref(stream);
		g_object_unref(fp);
		return "";
	}

	error_num = regcomp(&header_regex, "\\[playlist\\]", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		g_object_unref(stream);
		g_object_unref(fp);
		return "";
	}

	error_num = regcomp(&http, "http:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		g_object_unref(stream);
		g_object_unref(fp);
		return "";
	}

	error_num = regcomp(&https, "https:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		regfree(&http);
		g_object_unref(stream);
		g_object_unref(fp);
		return "";
	}

	error_num = regcomp(&ftp, "ftp:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		regfree(&http);
		regfree(&https);
		g_object_unref(stream);
		g_object_unref(fp);
		return "";
	}

	error_num = regcomp(&mms, "mms:\\/\\/", REG_ICASE);
	if (error_num) {
		regerror(error_num, &header_regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\n", err_msg);
		regfree(&regex);
		regfree(&file_regex);
		regfree(&header_regex);
		regfree(&http);
		regfree(&https);
		regfree(&ftp);
		g_object_unref(stream);
		g_object_unref(fp);
		return "";
	}

	char* line;

	//line = malloc(256 * sizeof(char));

	int header_found = 0;

	unsigned long size = 256;

	line = g_data_input_stream_read_line(data_stream, &size, NULL, NULL);

	error_num = regexec(&header_regex, line, 0, NULL, 0);
	if (!error_num) {
		header_found = 1;
	}

	char* string;

	//string = "opps";

	while ((line = g_data_input_stream_read_line(data_stream, &size, NULL, NULL)) != NULL) {

		if (header_found == 1) {
			error_num = regexec(&file_regex, line, 0, NULL, 0);
			if (!error_num) {
				error_num = regexec(&regex, line, 0, NULL, 0);
				if (!error_num) {
					
					if (regexec(&http, line, 0, NULL, 0) == 0) {
						string = strstr(line, "http");
						break;
					}
					else if (regexec(&https, line, 0, NULL, 0) == 0) {
						string = strstr(line, "https");
						break;
					}
					else if (regexec(&ftp, line, 0, NULL, 0) == 0) {
						string = strstr(line, "ftp");
						break;
					}
					else if (regexec(&mms, line, 0, NULL, 0) == 0) {
						string = strstr(line, "mms");
						break;
					}
				}
			}
		}
	}

	regfree(&regex);
	regfree(&file_regex);
	regfree(&header_regex);
	regfree(&http);
	regfree(&https);
	regfree(&ftp);

	g_object_unref(stream);
	g_object_unref(fp);

	char* done;

	done = malloc(256 * sizeof(char));

	strcpy(done, string);

	free(line);

	return done;
}

int genaric_regex(const char* string, const char* regex_string) {
	regex_t regex;
	int error_num;
	char err_msg[256];

	error_num = regcomp(&regex, regex_string, REG_ICASE);
	if (error_num) {
		regerror(error_num, &regex, err_msg, sizeof(err_msg));
		fprintf(stderr, "error doing regex: %s\r\n", err_msg);
		return -1;
	}

	error_num = regexec(&regex, string, 0, NULL, 0);
	if (!error_num) {
		regfree(&regex);
		return 0;
	}

	regfree(&regex);

	return 1;
}
