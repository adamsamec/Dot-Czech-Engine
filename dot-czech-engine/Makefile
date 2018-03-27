TARGET = dot-czech-engine
HEADERS = app_config.h english.h czech.h language.h debug.h
SOURCES = app_config.c english.c czech.c language.c
CFLAGS = -g #-Wall -Werror -pedantic -pedantic-errors
CC = gcc

.PHONY: run clean

all: $(TARGET)

$(TARGET): main.c $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)

