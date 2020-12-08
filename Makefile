CFLAGS=-std=c11 -g
LDFLAGS=-static
CC=gcc
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

libadmin.exe: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

test:
	./test.sh

clean:
	rm -f libadmin.exe *.o *~ tmp*
.PHONY: test clean