TARGET = dot-engine-verificator
HEADERS = ./../dot-czech-engine/app_config.h ./../dot-czech-engine/czech.h ./../dot-czech-engine/language.h ./../dot-czech-engine/debug.h verification.h buf.h
SOURCES = ./../dot-czech-engine/app_config.c ./../dot-czech-engine/czech.c ./../dot-czech-engine/language.c
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

