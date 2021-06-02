#CC=gcc
CFLAGS?= -O2 -g
CPPFLAGS= $(shell pkg-config --cflags gtk+-3.0 gstreamer-1.0)
LDLIBS= $(shell pkg-config --libs gtk+-3.0 sqlite3 gstreamer-1.0)

SRCDIR= src
OBJDIR= obj
PREFIX?= /usr/local

build: $(OBJDIR) $(SRCDIR)/rajio.h $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/station_reader.o $(OBJDIR)/g-bus.o $(SRCDIR)/gtk/station_button.h $(OBJDIR)/gtk/station_button.o $(OBJDIR)/gui.o $(OBJDIR)/rajio_app.o $(OBJDIR)/helpers.o $(OBJDIR)/callbacks.o
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/station_reader.o $(OBJDIR)/g-bus.o $(OBJDIR)/gtk/station_button.o $(OBJDIR)/gui.o $(OBJDIR)/rajio_app.o $(OBJDIR)/helpers.o $(OBJDIR)/callbacks.o -o rajio $(LDLIBS) $(LDFLAGS)

$(OBJDIR):
	@mkdir $(OBJDIR)
	@mkdir $(OBJDIR)/gtk

$(OBJDIR)/main.o: $(SRCDIR)/main.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -Dstations_file="\"${PREFIX}/share/rajio/stations\"" -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(OBJDIR)/parser.o: $(SRCDIR)/parser.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/parser.c -o $(OBJDIR)/parser.o

$(OBJDIR)/station_reader.o: $(SRCDIR)/station_reader.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/station_reader.c -o $(OBJDIR)/station_reader.o

$(OBJDIR)/g-bus.o: $(SRCDIR)/g-bus.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/g-bus.c -o $(OBJDIR)/g-bus.o

$(OBJDIR)/gtk/station_button.o: $(SRCDIR)/gtk/station_button.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/gtk/station_button.c -o $(OBJDIR)/gtk/station_button.o

$(OBJDIR)/gui.o: $(SRCDIR)/gui.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/gui.c -o $(OBJDIR)/gui.o

$(OBJDIR)/rajio_app.o: $(SRCDIR)/rajio_app.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/rajio_app.c -o $(OBJDIR)/rajio_app.o

$(OBJDIR)/helpers.o: $(SRCDIR)/helpers.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/helpers.c -o $(OBJDIR)/helpers.o

$(OBJDIR)/callbacks.o: $(SRCDIR)/callbacks.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $(SRCDIR)/callbacks.c -o $(OBJDIR)/callbacks.o

install: build
	@install -D rajio -t ${DESTDIR}${PREFIX}/bin
	@install -D stations -t ${DESTDIR}${PREFIX}/share/rajio

.PHONY: clean
clean:
	@-rm rajio 
	@-rm $(OBJDIR)/main.o 
	@-rm $(OBJDIR)/parser.o 
	@-rm $(OBJDIR)/station_reader.o 
	@-rm $(OBJDIR)/g-bus.o 
	@-rm $(OBJDIR)/gui.o
	@-rm $(OBJDIR)/gtk/station_button.o
	@-rm $(OBJDIR)/rajio_app.o
	@-rm $(OBJDIR)/helpers.o
	@-rm $(OBJDIR)/callbacks.o
	@-rmdir $(OBJDIR)/gtk
	@-rmdir $(OBJDIR)

.PHONY: uninstall

uninstall: 
	@rm -f ${DESTDIR}${PREFIX}/bin/rajio
	@rm -f ${DESTDIR}${PREFIX}/share/rajio/stations
	@rmdir ${DESTDIR}${PREFIX}/share/rajio