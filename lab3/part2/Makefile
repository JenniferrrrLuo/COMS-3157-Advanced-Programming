CC = gcc
CFLAGS = -g -Wall -I../part1
LDFLAGS = -L../part1
LDLIBS = -lmylist

revecho: revecho.o
revecho.o: revecho.c ../part1/mylist.h

.PHONY: clean
clean:
	rm -f *.o a.out core revecho

.PHONY: all
all: clean revecho

