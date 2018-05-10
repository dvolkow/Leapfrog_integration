CC=gcc
CFLAGS=-Wall -O2# -Wpedantic -O3
MPFRFLAG=-DLP_MPFR_CASE
DEBUGFLAG=-DLEAPFROG_DEBUG
LDFLAGS=-lm -lmpfr
SOURCES=leapfrog_main.c leapfrog_core.c leapfrog_utils.c leapfrog_io.c leapfrog_stats.c leapfrog_generator.c leapfrog_math.c leapfrog_parser.c leapfrog_cfg.c leapfrog_time.c
HEADERS=leapfrog.h leapfrog_types.h leapfrog_utils.h leapfrog_const.h leapfrog_io.h leapfrog_compile.h leapfrog_math.h leapfrog_precision.h leapfrog_stats.h leapfrog_core.h leapfrog_generator.h leapfrog_memory.h leapfrog_parser.h leapfrog_cfg.h leapfrog_time.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=leapfrog

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(MPFRFLAG) # $(DEBUGFLAG)

clean:
	rm $(OBJECTS) $(EXECUTABLE)

oclean:
	rm $(OBJECTS) 


install: oclean
	sudo mv ./leapfrog /usr/bin/leapfrog
#	sudo mkdir -p /usr/share/leapfrog
#	sudo cp ./lp.cfg /usr/share/leapfrog/lp.cfg


