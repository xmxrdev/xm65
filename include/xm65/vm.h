#pragma once

#include <stdbool.h>
#include <stdint.h>

#define RAM_SIZE 0x10000

enum {
    XM65_FLAG_C = 1u << 0,
    XM65_FLAG_Z = 1u << 1,
    XM65_FLAG_I = 1u << 2,
    XM65_FLAG_D = 1u << 3,
    XM65_FLAG_B = 1u << 4,
    XM65_FLAG_U = 1u << 5,
    XM65_FLAG_V = 1u << 6,
    XM65_FLAG_N = 1u << 7,
};

enum {
    XM65_VECTOR_NMI   = 0xFFFA,
    XM65_VECTOR_RESET = 0xFFFC,
    XM65_VECTOR_IRQ   = 0xFFFE,
    // XM65_VECTOR_ABORT     = 0xFFF8,
    // XM65_VECTOR_COP       = 0xFFF4,
};

typedef enum {
    XM65_VM_STATUS_IDLE,
    XM65_VM_STATUS_RUNNING,
    XM65_VM_STATUS_INTERRUPTED,
} XM65_VM_STATUS;

typedef struct XM65_6502 {
    uint8_t a, x, y, sp, p;
    uint16_t pc;
    uint64_t cycles;
} XM65_6502;

typedef struct XM65_RAM {
    uint8_t data[RAM_SIZE];
} XM65_RAM;

typedef struct XM65_VM {
    XM65_RAM ram;
    XM65_6502 cpu;
    XM65_VM_STATUS status;
} XM65_VM;

void XM65_PrintCPU(XM65_VM*);
void XM65_UpdateFlags(XM65_VM*, uint16_t, uint8_t, uint16_t);
uint16_t XM65_ReadVector(XM65_VM*, uint16_t);
uint8_t XM65_ReadOperand(XM65_VM*);
uint8_t XM65_ReadIndirect(XM65_VM*, uint8_t, uint8_t);
uint16_t XM65_AddCarry(XM65_VM*, uint8_t);
uint16_t XM65_SubstractCarry(XM65_VM*, uint8_t);
void XM65_ProgramVM(XM65_VM*, const char*);
void XM65_Power_VM(XM65_VM*);
void XM65_ResetVM(XM65_VM*);
XM65_VM_STATUS XM65_RunVM(XM65_VM*);
void XM65_StackPush(XM65_VM*, uint8_t);
uint8_t XM65_StackPull(XM65_VM*);
