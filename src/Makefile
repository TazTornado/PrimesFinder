#############################################################################
# Compilation Options:														#
# 	  Command					Action										#
# ------------------------------------------------------------------------- #
# >> make all	  	=> all source files and creates all executables			#
# >> make myprime 	=> myprime and its dependencies							#
# >> make Inode		=> Inode and its dependencies							#
# >> make prime1	=> only prime1											#
# >> make prime2	=> only prime2											#
# >> make prime3	=> only prime3											#
# >> make clean		=> removes all object and all executable files			#
#############################################################################

IDIR = ../include
ODIR = ../obj
BDIR = ../bin
CC = gcc
CFLAGS = -ggdb -Wall -I$(IDIR)


_DEPS = subrange.h datatypes.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.c $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)

all: Inode prime1 prime2 prime3 myprime

Inode: $(ODIR)/Inode.o $(ODIR)/subrange.o 
	@echo creating Inode..
	@$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

prime1: $(ODIR)/prime1.o
	@echo creating prime1..
	@$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

prime2: $(ODIR)/prime2.o
	@echo creating prime2..
	@$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lm

prime3: $(ODIR)/prime3.o
	@echo creating prime3..
	@$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) -lm

myprime: $(ODIR)/myprime.o $(ODIR)/subrange.o 
	@echo creating myprime..
	@$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	@echo Removing object files and executable files
	@rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BDIR)/*
	@echo Done!
