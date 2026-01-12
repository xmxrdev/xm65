TARGET = xm65

CC = cc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Iinclude

DIR_INCLUDE := include
DIR_SRC := src
DIR_BUILD := build
DIR_BIN := bin

SRCS := $(wildcard $(DIR_SRC)/*.c)
OBJS := $(SRCS:$(DIR_SRC)/%.c=$(DIR_BUILD)/%.o)

all: $(DIR_BIN)/$(TARGET)

$(DIR_BIN)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(DIR_BUILD)/%.o: $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BUILD) $(DIR_BIN):
	mkdir -p $@

clean:
	rm -f $(OBJS) $(DIR_BIN)/$(TARGET)

.PHONY: all clean
