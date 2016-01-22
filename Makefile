OS=$(shell uname)
ifeq ($(OS), Linux)
	CVERS=gnu99
else
	CVERS=c99
endif

CFLAGS=-c -Wall -pedantic -std=$(CVERS)
LDFLAGS=
CC=gcc
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hospital

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
