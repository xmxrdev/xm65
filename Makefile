ASM_ROOT := asm

SRC_DIR := $(ASM_ROOT)/src
OBJ_DIR := $(ASM_ROOT)/build
BIN_DIR := $(ASM_ROOT)/bin

ASM_SRCS := $(wildcard $(SRC_DIR)/*.asm)
ASM_BINS := $(patsubst $(SRC_DIR)/%.asm,$(BIN_DIR)/%.bin,$(ASM_SRCS))

asm: $(ASM_BINS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(OBJ_DIR)
	ca65 --cpu 6502 $< -o $@

$(BIN_DIR)/%.bin: $(OBJ_DIR)/%.o
	@mkdir -p $(BIN_DIR)
	ld65 -o $@ $< -C $(ASM_ROOT)/memory.cfg
 
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: asm clean
