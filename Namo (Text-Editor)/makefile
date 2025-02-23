CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb `pkg-config --cflags sdl2 freetype2 harfbuzz`
LIBS = `pkg-config --libs sdl2 SDL2_ttf freetype2 harfbuzz` -lm

te: main.c la.c
	$(CC) $(CFLAGS) -o te main.c   $(LIBS)
