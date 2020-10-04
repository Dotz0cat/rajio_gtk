CC=clang
CFLAGS= -g $(shell pkg-config --cflags gtk+-3.0 gstreamer-1.0)
LDLIBS= $(shell pkg-config --libs gtk+-3.0 sqlite3 gstreamer-1.0)

SRCDIR= src

OBJ= main.o parser.o station_reader.o g-bus.o
SRC= $(SRCDIR)/main.c $(SRCDIR)/parser.c $(SRCDIR)/station_reader.c $(SRCDIR)/g-bus.c

build: $(OBJ)
	$(CC) $(CFLAGS) -O2 $(OBJ) -o rajio $(LDLIBS) 

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

.PHONY : clean
clean :
	-rm rajio $(OBJ)