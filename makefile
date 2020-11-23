#CC=gcc
CFLAGS= -g $(shell pkg-config --cflags gtk+-3.0 gstreamer-1.0)
LDLIBS= $(shell pkg-config --libs gtk+-3.0 sqlite3 gstreamer-1.0)

SRCDIR= src
OBJDIR= obj
PREFIX?= /usr/local

build: $(OBJDIR) $(SRCDIR)/rajio.h $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/station_reader.o $(OBJDIR)/g-bus.o $(SRCDIR)/gtk/station_button.h $(OBJDIR)/gtk/station_button.o
	@$(CC) $(CFLAGS) -O2 $(OBJDIR)/main.o $(OBJDIR)/parser.o $(OBJDIR)/station_reader.o $(OBJDIR)/g-bus.o $(OBJDIR)/gtk/station_button.o -o rajio $(LDLIBS) 

$(OBJDIR):
	@mkdir $(OBJDIR)
	@mkdir $(OBJDIR)/gtk

$(OBJDIR)/main.o: $(SRCDIR)/main.c
	@$(CC) $(CFLAGS) -Dgtk_builder_file="\"${PREFIX}/share/rajio/rajio_gtk_v2.glade\"" -Dstations_file="\"${PREFIX}/share/rajio/stations\"" -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

$(OBJDIR)/parser.o: $(SRCDIR)/parser.c
	@$(CC) $(CFLAGS) -c $(SRCDIR)/parser.c -o $(OBJDIR)/parser.o

$(OBJDIR)/station_reader.o: $(SRCDIR)/station_reader.c
	@$(CC) $(CFLAGS) -c $(SRCDIR)/station_reader.c -o $(OBJDIR)/station_reader.o

$(OBJDIR)/g-bus.o: $(SRCDIR)/g-bus.c
	@$(CC) $(CFLAGS) -c $(SRCDIR)/g-bus.c -o $(OBJDIR)/g-bus.o

$(OBJDIR)/gtk/station_button.o: $(SRCDIR)/gtk/station_button.c
	@$(CC) $(CFLAGS) -c $(SRCDIR)/gtk/station_button.c -o $(OBJDIR)/gtk/station_button.o

install: build
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	install rajio ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/share/rajio
	install stations ${DESTDIR}${PREFIX}/share/rajio
	install rajio_gtk_v2.glade ${DESTDIR}${PREFIX}/share/rajio

.PHONY: clean
clean:
	-rm rajio 
	-rm $(OBJDIR)/main.o 
	-rm $(OBJDIR)/parser.o 
	-rm $(OBJDIR)/station_reader.o 
	-rm $(OBJDIR)/g-bus.o 
	-rm $(OBJDIR)/gtk/station_button.o
	-rmdir $(OBJDIR)/gtk
	-rmdir $(OBJDIR)

.PHONY: uninstall

uninstall: 
	rm -f ${DESTDIR}${PREFIX}/bin/rajio
	rm -f ${DESTDIR}${PREFIX}/share/rajio/stations
	rm -f ${DESTDIR}${PREFIX}/share/rajio/rajio_gtk_v2.glade
	rmdir ${DESTDIR}${PREFIX}/share/rajio