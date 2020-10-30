CC=clang
CFLAGS= -g $(shell pkg-config --cflags gtk+-3.0 gstreamer-1.0)
LDLIBS= $(shell pkg-config --libs gtk+-3.0 sqlite3 gstreamer-1.0)

SRCDIR= src
PREFIX= /usr/local

OBJ= main.o parser.o station_reader.o g-bus.o
SRC= $(SRCDIR)/main.c $(SRCDIR)/parser.c $(SRCDIR)/station_reader.c $(SRCDIR)/g-bus.c

build: $(OBJ) $(SRCDIR)/rajio.h
	$(CC) $(CFLAGS) -O2 $(OBJ) -o rajio $(LDLIBS) 

$(OBJ): $(SRC) $(SRCDIR)/rajio.h
	$(CC) $(CFLAGS) -c $(SRC)

install: build
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install rajio ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${PREFIX}/share/rajio
	install stations ${DESTDIR}${PREFIX}/share/rajio
	install rajio_gtk_v2.glade ${DESTDIR}${PREFIX}/share/rajio

.PHONY : clean uninstall
clean :
	-rm rajio $(OBJ)

uninstall: 
	rm -f ${DESTDIR}${PREFIX}/bin/rajio
	rm -f ${DESTDIR}${PREFIX}/share/rajio/stations
	rm -f ${DESTDIR}${PREFIX}/share/rajio/rajio_gtk_v2.glade