CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
SOURCES=leapfrog_main.c leapfrog_core.c 
HEADERS=leapfrog.h leapfrog_types.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=leapfrog

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm $(OBJECTS) $(EXECUTABLE)
