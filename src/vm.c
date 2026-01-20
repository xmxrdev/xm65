#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "xm65/vm.h"
#include "xm65/utils.h"

void XM65_PrintCPU(XM65_VM *vm) {
    printf("A=$%X X=$%X Y=$%X\nSP=$%X PC=$%X D=$%X\nNV-BDIZC\n%s (0x%X)\n\n", vm->cpu.a, vm->cpu.x, vm->cpu.y, vm->cpu.sp, vm->cpu.pc, vm->ram.data[vm->cpu.pc], XM65_ByteToBin(vm->cpu.p), vm->cpu.p);
}

void XM65_UpdateFlags(XM65_VM *vm, uint16_t M, uint8_t R, uint16_t RV) {
    vm->cpu.p &= (uint8_t) ~(XM65_FLAG_C | XM65_FLAG_Z | XM65_FLAG_V | XM65_FLAG_N);

    if (RV > 0xFF)                                          vm->cpu.p |= XM65_FLAG_C;
    if (R == 0)                                             vm->cpu.p |= XM65_FLAG_Z;
    if (R & 0x80)                                           vm->cpu.p |= XM65_FLAG_N;
    if (((~(vm->cpu.a ^ M) & (vm->cpu.a ^ R)) & 0x80) != 0) vm->cpu.p |= XM65_FLAG_V;
}

uint16_t XM65_ReadVector(XM65_VM *vm, uint16_t lo) {
    return vm->ram.data[lo] | (uint16_t)(vm->ram.data[lo + 1u] << 8);
}

uint8_t XM65_ReadOperand(XM65_VM *vm) {
    return vm->ram.data[vm->cpu.pc++];
}

uint8_t XM65_ReadIndirect(XM65_VM *vm, uint8_t OB, uint8_t OA) {
    uint8_t ZP = XM65_ReadOperand(vm);
    uint8_t E = (ZP + OB) & 0xFF;
    uint16_t ADDR;
    ADDR  = vm->ram.data[E];
    ADDR |= vm->ram.data[(E + 1) & 0xFF] << 8;
    uint16_t MO = (ADDR + OA) & 0xFFFF;

    if ((ADDR & 0xFF00) != (MO & 0xFF00)) vm->cpu.cycles += 1;

    return vm->ram.data[MO];
}

uint16_t XM65_ReadAbsolute(XM65_VM *vm, uint8_t O, bool deref) {
    uint16_t ADDR;
    ADDR = XM65_ReadOperand(vm);
    ADDR |= XM65_ReadOperand(vm) << 8;
    uint16_t E = (ADDR + O) & 0xFFFF;

    if ((ADDR & 0xFF00) != (E & 0xFF00)) vm->cpu.cycles += 1;

    uint16_t V = deref ? vm->ram.data[E] : E;
    
    return V;
}

uint16_t XM65_AddCarry(XM65_VM *vm, uint8_t M) {
    return (uint16_t)((uint16_t)vm->cpu.a + (uint16_t)M + (vm->cpu.p & XM65_FLAG_C) ? 1u : 0u);
}

uint16_t XM65_SubstractCarry(XM65_VM *vm, uint8_t M) {
    return (uint16_t)((uint16_t)vm->cpu.a - (uint16_t)M - (vm->cpu.p & XM65_FLAG_C) ? 0u : 1u);
}

void XM65_StackPush(XM65_VM *vm, uint8_t value) {
    vm->ram.data[0x0100 + vm->cpu.sp--] = value;
}

uint8_t XM65_StackPull(XM65_VM *vm) {
    return vm->ram.data[0x0100 + ++vm->cpu.sp];
}

void XM65_ProgramVM(XM65_VM *vm, const char *filename) {
    char *file = NULL; long filesize;
    XM65_OpenFile(filename, &file, &filesize);

    // Copying PRG memory (excluding last 6 bytes for vectors)
    int program_size = (int) filesize - 6;
    if (program_size > 0) {
        for (int i = 0; i < program_size; i++) {
            vm->ram.data[0x0200 + i] = (uint8_t) file[i];
        }
    }

    // Copying VECTOR memory
    vm->ram.data[XM65_VECTOR_IRQ     ] = (uint8_t) file[program_size +0];
    vm->ram.data[XM65_VECTOR_IRQ   +1] = (uint8_t) file[program_size +1];
    vm->ram.data[XM65_VECTOR_RESET   ] = (uint8_t) file[program_size +2];
    vm->ram.data[XM65_VECTOR_RESET +1] = (uint8_t) file[program_size +3];
    vm->ram.data[XM65_VECTOR_NMI     ] = (uint8_t) file[program_size +4];
    vm->ram.data[XM65_VECTOR_NMI   +1] = (uint8_t) file[program_size +5];

    XM65_CloseFile(file);
}

void XM65_Power_VM(XM65_VM *vm) {
    /// Simulating undefined state of CPU registers and RAM contents
    // Randomize CPU registers
    vm->cpu.a  = (uint8_t)(rand() & 0xFF);
    vm->cpu.x  = (uint8_t)(rand() & 0xFF);
    vm->cpu.y  = (uint8_t)(rand() & 0xFF);
    vm->cpu.p  = (uint8_t)(rand() & 0xFF);
    vm->cpu.sp = (uint8_t)(rand() & 0xFF);

    // Randomize RAM contents
    for (size_t i = 0; i < RAM_SIZE; i++) {
        vm->ram.data[i] = (uint8_t)(rand() & 0xFF);
    }
}

void XM65_ResetVM(XM65_VM *vm) {
    // Internal reset, no bus access +3 cycles
    vm->cpu.pc = XM65_ReadVector(vm, XM65_VECTOR_RESET); // +3 cycles

    // Set interrupt disable flag: disable IRQ's, then continue
    vm->cpu.p |= XM65_FLAG_U | XM65_FLAG_B;

    // Reset cpu cycles counter
    vm->cpu.cycles = 6;

    vm->status = XM65_VM_STATUS_IDLE;
}

