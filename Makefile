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

asm:
	ca65 --cpu 6502 asm/test01.asm -o asm/build/test01.o; ld65 -o asm/bin/test01.bin asm/build/test01.o -C asm/memory.cfg
	ca65 --cpu 6502 asm/test02.asm -o asm/build/test02.o; ld65 -o asm/bin/test02.bin asm/build/test02.o -C asm/memory.cfg
	ca65 --cpu 6502 asm/test03.asm -o asm/build/test03.o; ld65 -o asm/bin/test03.bin asm/build/test03.o -C asm/memory.cfg

clean:
	rm -f $(OBJS) $(DIR_BIN)/$(TARGET)

.PHONY: all asm clean
