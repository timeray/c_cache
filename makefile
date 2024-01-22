CC=gcc
INCDIRS=-I.
LIBDIRS=-L/ucrt64/lib
LIBINC=-lcheck
OPT=-O2
CFLAGS=-Wall $(INCDIRS) $(LIBDIRS) $(OPT)

CFILES=page.c clist.c tests/test.c
OBJECTS=page.o clist.o tests/test.o

BINARY_TEST=test_bin

# Rules
# First rule will be executed when make is called
all: $(BINARY_TEST)

# $@ - ref to lefthand side, $^ - ref to righthand side
$(BINARY_TEST): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBINC)

# % - wildcard
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^


clean:
	rm -rf $(OBJECTS) $(BINARY_TEST)
