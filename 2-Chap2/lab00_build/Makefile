# Lab 00 - Vi du bien dich nhieu tep nguon (Chapter 2, slide 4-6)
CC      ?= gcc
CFLAGS  = -std=gnu11 -Wall -Wextra -g
LDLIBS  = -lm

all: hello calc

hello: hello.c
	$(CC) $(CFLAGS) -o $@ $<

# Bien dich tung tep thanh .o roi lien ket (tuong tu slide 5)
calc: main.o mathutil.o
	$(CC) $(CFLAGS) -o $@ main.o mathutil.o $(LDLIBS)

main.o: main.c mathutil.h
	$(CC) $(CFLAGS) -c main.c

mathutil.o: mathutil.c mathutil.h
	$(CC) $(CFLAGS) -c mathutil.c

clean:
	rm -f hello calc *.o

.PHONY: all clean
