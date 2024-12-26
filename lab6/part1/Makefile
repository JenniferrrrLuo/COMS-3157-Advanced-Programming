# Makefile for lab 6, part 1

CC      = gcc
CFLAGS  = -g -Wall -I../../lab3/solutions/part1
LDFLAGS = -L../../lab3/solutions/part1
LDLIBS  = -lmylist

mdb-lookup-server: mdb-lookup-server.o mdb.o

mdb-lookup-server.o: mdb.h

mdb.o: mdb.h

.PHONY: clean
clean:
	rm -f *.o a.out mdb-lookup-server

.PHONY: all
all: clean default
