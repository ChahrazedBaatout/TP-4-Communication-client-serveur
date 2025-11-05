SRC = $(wildcard *.c)
BIN = $(patsubst %.c,%,$(SRC))

CFLAGS += -Wall -Wextra -g
LIBS = libseg.a
#CFLAGS += --std=c99

all: $(BIN)

%: %.c $(LIBS)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~

indent:
	indent -linux -i4 -nut -ts2 *.c

.PHONY: all clean indent
