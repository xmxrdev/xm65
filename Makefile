CC=cc

SRC_DIR=src

all:
	$(CC) $(SRC_DIR)/main.c -o xm65

.PHONY: all
