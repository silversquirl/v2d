.PHONY: all clean

EXAMPLES = hello_world mouse_picking

CC := gcc -std=c99 -pedantic
CFLAGS := -Wall -Werror -I../include -O2
LDFLAGS := -L../ -lv2d -lSDL2 -lm

ifeq ($(OS),Windows_NT)
	LDFLAGS := -lmingw32 -lSDL2main $(LDFLAGS)
endif

all: $(EXAMPLES)

clean:
	rm -f $(EXAMPLES)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
