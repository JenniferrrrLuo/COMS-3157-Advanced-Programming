CC = gcc
CFLAGS = -g -Wall

twecho: twecho.o
twecho.o: twecho.c

.PHONY: clean
clean:
	rm -f *.o a.out core twecho
.PHONY: all
all: clean twecho
