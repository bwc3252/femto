CC = gcc
CFLAGS += -std=c99 -pedantic -Wall -ggdb
LFLAGS = -lncurses
TARGET = build/femto
RM = rm
SOURCES = src/main.c src/buffer.c src/line.c src/display.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: directories logfile $(TARGET)

logfile:
	touch log.txt

directories: build

build:
	mkdir -p build/

clean:
	$(RM) $(TARGET) $(OBJECTS) log.txt

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
