# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
PKG_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_ttf freetype2 harfbuzz)
PKG_LIBS = $(shell pkg-config --libs sdl2 SDL2_ttf freetype2 harfbuzz)

# Source files and target
SRC = main.c la.c lexer.c editor.c
OBJ = $(SRC:.c=.o)
TARGET = NAmo

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(PKG_LIBS) -lm

%.o: %.c
	$(CC) $(CFLAGS) $(PKG_CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJ) $(TARGET)
