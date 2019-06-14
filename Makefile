CC= gcc
CFLAGS+=-std=c99 -pedantic -Wall -ggdb
TARGET = build/femto
RM = rm
SOURCES = src/main.c src/buffer.c src/line.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean directories

all: $(TARGET)

directories: build

build:
	mkdir -p build/

clean:
	$(RM) $(TARGET) $(OBJECTS)

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

build/:
	mkdir build/
