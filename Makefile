SRCDIR := src
INC := -I include
BUILDDIR := build
TARGET := bin/hospital

SOURCES := $(shell find $(SRCDIR) -type f -name *.c)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o)) 

OS=$(shell uname)
ifeq ($(OS), Linux)
	CVERS=gnu99
else
	CVERS=c99
endif

CFLAGS=-c -Wall -pedantic -std=$(CVERS)
LDFLAGS=-lm
CC=gcc


all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -r $(BUILDDIR) $(TARGET)
