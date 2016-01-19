CC=gcc
CFLAGS=-c -Wall -pedantic
LDFLAGS=
SOURCES=hospital.c config.c comm.c generatorePazienti.c triage.c reparto.c prestazione.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hospital

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
