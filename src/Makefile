CC=gcc
CFLAGS=-Wall -O2 # -DLP_MPFR_CASE
LDFLAGS=-lm #-lmpfr
SOURCES=leapfrog_main.c leapfrog_core.c leapfrog_utils.c leapfrog_io.c leapfrog_stats.c leapfrog_generator.c # leapfrog_math.c
HEADERS=leapfrog.h leapfrog_types.h leapfrog_utils.h leapfrog_const.h leapfrog_io.h leapfrog_compile.h leapfrog_math.h leapfrog_precision.h leapfrog_stats.h leapfrog_core.h leapfrog_generator.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=leapfrog

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm $(OBJECTS) $(EXECUTABLE)
