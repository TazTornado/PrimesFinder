# This is the secondary Makefile that handles the secondary utilities of the project #

CC=gcc
CFLAGS=-ggdb -Wall

ODIR=../../obj

_OBJ = mytiming.o prime1.o prime2.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: prime1 prime2 mytiming

prime1: $(ODIR)/prime1.o
	$(CC) -o $@ $^ $(CFLAGS)

prime2: $(ODIR)/prime2.o
	$(CC) -o $@ $^ $(CFLAGS) -lm

mytiming: $(ODIR)/mytiming.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core
	rm -f prime1 prime2 mytiming a.out
