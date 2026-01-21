APPNAME=myprog.exe
SRCS=main.cpp
#i686-w64-mingw32-gcc
CC=g++ -finput-charset=UTF-8 -fextended-identifiers
CFLAGS=-o $(APPNAME) -Wall -O2
ALLEGRO=`pkg-config --cflags --libs allegro-5 allegro_ttf-5 allegro_primitives-5 allegro_physfs-5 allegro_memfile-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_dialog-5 allegro_color-5 allegro_audio-5 allegro_acodec-5`

.PHONY: all main clean

all: clean main

main:
	$(CC) $(CFLAGS) $(SRCS) $(ALLEGRO)

clean:
	rm -rf *.o $(APPNAME)