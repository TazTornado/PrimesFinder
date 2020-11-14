# This is the general Makefile that handles all source files of the project #

IDIR =../include
ODIR = ../obj
SECDIR = ./secondary
CC = gcc
CFLAGS = -ggdb -Wall -I$(IDIR)


_DEPS = dummy.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: utilities myprime

utilities:
	$(MAKE) -C $(SECDIR)

myprime: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f myprime
	cd secondary; make clean
