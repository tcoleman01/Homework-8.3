CC=gcc
CFLAGS=-O2 -std=c11 -Wall -Wextra -pedantic
LDFLAGS=

OBJS=main.o skiplist.o

all: skipdemo

skipdemo: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.c skiplist.h
skiplist.o: skiplist.c skiplist.h

clean:
	rm -f $(OBJS) skipdemo
