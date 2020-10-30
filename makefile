CC=clang
CFLAGS= -g $(shell pkg-config --cflags gtk+-3.0 gstreamer-1.0)
LDLIBS= $(shell pkg-config --libs gtk+-3.0 sqlite3 gstreamer-1.0)

SRCDIR= src
PREFIX?= /usr/local

build: $(OBJ) $(SRCDIR)/rajio.h main.o parser.o station_reader.o g-bus.o
	@$(CC) $(CFLAGS) -O2 main.o parser.o station_reader.o g-bus.o -o rajio $(LDLIBS) 

main.o: $(SRCDIR)/main.c
	@$(CC) $(CFLAGS) -Dgtk_builder_file="\"${PREFIX}/share/rajio/rajio_gtk_v2.glade\"" -Dstations_file="\"${PREFIX}/share/rajio/stations\"" -c $(SRCDIR)/main.c

parser.o: $(SRCDIR)/parser.c
	@$(CC) $(CFLAGS) -c $(SRCDIR)/parser.c

station_reader.o: $(SRCDIR)/station_reader.c
	@$(CC) $(CFLAGS) -c $(SRCDIR)/station_reader.c

g-bus.o: $(SRCDIR)/g-bus.c
	@$(CC) $(CFLAGS) -c $(SRCDIR)/g-bus.c

install: build
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	install rajio ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/share/rajio
	install stations ${DESTDIR}${PREFIX}/share/rajio
	install rajio_gtk_v2.glade ${DESTDIR}${PREFIX}/share/rajio

.PHONY : clean uninstall
clean :
	-rm rajio main.o parser.o station_reader.o g-bus.o 

uninstall: 
	rm -f ${DESTDIR}${PREFIX}/bin/rajio
	rm -f ${DESTDIR}${PREFIX}/share/rajio/stations
	rm -f ${DESTDIR}${PREFIX}/share/rajio/rajio_gtk_v2.glade
	rmdir ${DESTDIR}${PREFIX}/share/rajio