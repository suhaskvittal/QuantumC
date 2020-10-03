CC=gcc

IDIR=../include
ODIR=../bin

CFLAGS=-I$(IDIR)

_DEPS=qubit.h gates.h storage.h qft.h
DEPS=$(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ=qubit.o gates.o storage.o qft.o
OBJ=$(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) main.c -o $@ $^

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~