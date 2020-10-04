#include "rajio.h"

//prototypes
void set_message_handlers(GstBus *bus, const char* sql_file);
void error_handler(GstBus *bus, GstMessage *msg, gpointer data);
void warn_handler(GstBus *bus, GstMessage *msg, gpointer data);
void eos_handler(GstBus *bus, GstMessage *msg, gpointer data);
void have_type(GstElement *typefind, guint probability, GstCaps *caps, gpointer user_data);
void deep_element_stuff(GstBin* self, GstBin* sub_bin, GstElement* element, gpointer data);
void element_stuff(GstBin* self, GstElement* element, gpointer data);

//external prototypes
extern int get_highest_id(char* file_name);
extern int start_playing(int station_id);
extern int genaric_regex(const char* string, const char* regex_string);

void set_message_handlers(GstBus *bus, const char* sql_file) {
	gst_bus_add_signal_watch_full(bus, G_PRIORITY_DEFAULT);
	g_signal_connect(bus, "message::error", G_CALLBACK(error_handler), NULL);
	g_signal_connect(bus, "message::warning", G_CALLBACK(warn_handler), NULL);
	g_signal_connect(bus, "message::eos", G_CALLBACK(eos_handler), sql_file);
	g_signal_connect(pipeline, "deep-element-added", G_CALLBACK(deep_element_stuff), NULL);
	g_signal_connect(pipeline, "element-added", G_CALLBACK(element_stuff), NULL);

}

void error_handler(GstBus *bus, GstMessage *msg, gpointer data) {
	GError *err;
    gchar *debug;

    gst_message_parse_error(msg, &err, &debug);
    printf("Error: %s\n", err->message);
    g_error_free(err);
    g_free(debug);
}

void warn_handler(GstBus *bus, GstMessage *msg, gpointer data) {
	GError *err;
    gchar *debug;

    gst_message_parse_warning(msg, &err, &debug);
    printf("warn: %s\n", err->message);
    g_error_free(err);
    g_free(debug);
}

void eos_handler(GstBus *bus, GstMessage *msg, gpointer data) {
	gst_element_set_state(pipeline, GST_STATE_READY);
	if (most_recent_id == get_highest_id(data)) {
		most_recent_id = 1;
	}
	else {
		most_recent_id++;
	}

	if (start_playing(most_recent_id) != 0) {
		fprintf(stderr, "There was a error\r\n");
	}

	return;
}

void have_type(GstElement *typefind, guint probability, GstCaps *caps, gpointer user_data) {
	//printf("have-type signal emited\r\n");
	//add as debug
}

void deep_element_stuff(GstBin* self, GstBin* sub_bin, GstElement* element, gpointer data) {
	//printf("added to deep pipeline: %s\r\n", gst_object_get_name(GST_OBJECT(element)));
	//add as debug

	if (genaric_regex(gst_object_get_name(GST_OBJECT(element)), "TypeFindElement") == 0) {
		g_signal_connect(element, "have-type", G_CALLBACK(have_type), NULL);
	}
	else if (genaric_regex(gst_object_get_name(GST_OBJECT(element)), "tsdemux") == 0) {
		g_object_set(G_OBJECT(element), "emit-stats", TRUE, NULL);
		g_object_set(G_OBJECT(element), "ignore-pcr", TRUE, NULL);
	}

	//GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipe");
}

void element_stuff(GstBin* self, GstElement* element, gpointer data) {
	//printf("added to pipeline: %s\r\n", gst_object_get_name(GST_OBJECT(element)));
	//add as debug

	if (genaric_regex(gst_object_get_name(GST_OBJECT(element)), "TypeFindElement") == 0) {
		g_signal_connect(element, "have-type", G_CALLBACK(have_type), NULL);
	}
}
