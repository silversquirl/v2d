.PHONY: all clean

EX_SRC = $(wildcard *.c)
EXAMPLES = $(patsubst %.c,%,$(EX_SRC))

CC := gcc -std=c99 -pedantic
CFLAGS := -Wall -Werror -I../include -O2
LDFLAGS := -L../ -lv2d -lSDL2 -lm

ifeq ($(OS),Windows_NT)
	LDFLAGS := -lmingw32 -lSDL2main $(LDFLAGS)
endif

all: $(EXAMPLES)

clean:
	rm -f $(EXAMPLES)

%: %.c ../libv2d.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)


../libv2d.a:
	$(MAKE) -C ..
