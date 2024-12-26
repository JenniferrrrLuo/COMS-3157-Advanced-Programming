CC = gcc
CFLAGS = -g -Wall

mylist-test: libmylist.a mylist-test.o
mylist-test.o: mylist-test.c mylist.h
libmylist.a: mylist.o
	ar rcs libmylist.a mylist.o
mylist.o: mylist.c mylist.h

.PHONY: clean
clean:
	rm -f *.o a.out core mylist-test libmylist.a

.PHONY: all
all: clean mylist-test

