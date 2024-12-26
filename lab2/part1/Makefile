CC = gcc
CFLAGS = -g -Wall

isort: isort.o
isort.o: isort.c

.PHONY: clean
clean:
	rm -f *.o a.out core isort
.PHONY: all
all: clean isort
