# Project: Graphical Calculator using C and GTK+
# File:    Makefile
# Author:  Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
# License: MIT (see LICENSE file for details)
#
# Usage:
#   make        # build the gtk_calculator binary
#   make clean  # remove object files and binary

CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 `pkg-config --cflags gtk+-3.0`
LDFLAGS := `pkg-config --libs gtk+-3.0` -lm

SRC     := src/main.c \
           src/ui.c \
           src/calculator.c

OBJ     := $(SRC:.c=.o)
TARGET  := gtk_calculator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
