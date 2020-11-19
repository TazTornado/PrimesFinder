#############################################################################
# Compilation Options:																	#
# 	  Command					Action										#
# ------------------------------------------------------------------------- #
# >> make all	  	=> all source files and creates all executables			#
# >> make myprime 	=> myprime and its dependencies							#
# >> make Inode		=> Inode and its dependencies							#
# >> make prime1	=> only prime1											#
# >> make prime2	=> only prime2											#
# >> make clean		=> removes all object and all executable files			#
#############################################################################

IDIR =../include
ODIR = ../obj
BDIR = ../bin
CC = gcc
CFLAGS = -ggdb -Wall -I$(IDIR)


_DEPS = subrange.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: Inode prime1 prime2 mytiming myprime

Inode: $(ODIR)/Inode.o $(ODIR)/subrange.o
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

prime1: $(ODIR)/prime1.o
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

prime2: $(ODIR)/prime2.o
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lm

mytiming: $(ODIR)/mytiming.o
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

myprime: $(ODIR)/myprime.o $(ODIR)/subrange.o
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BDIR)/*
	rm -f myprime Inode prime1 prime2 mytiming
