.PHONY: all clean

BUILD_DIR ?= build
SRC_DIR ?= src
HEADER_DIR ?= include

CC := gcc -std=c99 -pedantic
CFLAGS := -Wall -Werror -I$(HEADER_DIR)/ -O2
LDFLAGS :=

AR := ar

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
