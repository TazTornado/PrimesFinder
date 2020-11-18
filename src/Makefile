# This is the general Makefile that handles all source files of the project #

IDIR =../include
ODIR = ../obj
WDIR = ./Workers
INNERDIR = ./InnerNodes
CC = gcc
CFLAGS = -ggdb -Wall -I$(IDIR)


_DEPS = subrange.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o subrange.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: utilities myprime

utilities:
	$(MAKE) -C $(WDIR)
	$(MAKE) -C $(INNERDIR)


myprime: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f myprime
	cd $(WDIR); make clean
	cd $(INNERDIR); make clean
