.PHONY: all clean

CC ?= gcc
AR ?= ar
S2C ?= sdl2-config

BUILD_DIR ?= build
SRC_DIR ?= src
HEADER_DIR ?= include

CFLAGS := -std=c99 -pedantic -Wall -Werror -I$(HEADER_DIR)/ -O2 $(shell $(S2C) --cflags)
LDFLAGS :=

HEADERS := $(HEADER_DIR)/v2d.h $(wildcard $(HEADER_DIR)/v2d/*.h)
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

all: libv2d.a

clean:
	rm -rf $(BUILD_DIR)/
	rm -f libv2d.a

libv2d.a: $(OBJECTS)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
