# This Makefile handles the programs overlaying the inner nodes of the hierarchy tree #

CC=gcc
CFLAGS=-ggdb -Wall

ODIR=../../obj

_OBJ = Inodes.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: Inodes

Inodes: $(ODIR)/Inodes.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core
	rm -f Inodes a.out
