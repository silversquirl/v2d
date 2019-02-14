.PHONY: all clean

CC := gcc -std=c99 -pedantic
CFLAGS := -Wall -Werror -I../include -O2
LDFLAGS := -L../ -lv2d -lSDL2 -lm

ifeq ($(OS),Windows_NT)
	LDFLAGS := -lmingw32 -lSDL2main $(LDFLAGS)
endif

all: hello_world

clean:
	rm -f hello_world

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
