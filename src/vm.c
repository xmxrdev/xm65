#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "vm.h"
#include "utils.h"

void XM65_PrintCPU(XM65_VM *vm) {
    printf("A=$%X X=$%X Y=$%X\nSP=$%X PC=$%X D=$%X\nNV-BDIZC\n%s (0x%X)\n\n", vm->cpu.a, vm->cpu.x, vm->cpu.y, vm->cpu.sp, vm->cpu.pc, vm->ram.data[vm->cpu.pc], XM65_ByteToBin(vm->cpu.p), vm->cpu.p);
}

void XM65_UpdateFlags(XM65_VM *vm, uint8_t M, uint8_t R, uint16_t RV) {
    vm->cpu.p &= ~(XM65_FLAG_C | XM65_FLAG_Z | XM65_FLAG_V | XM65_FLAG_N);

    if (RV > 0xFF)                                          vm->cpu.p |= XM65_FLAG_C;
    if (R == 0)                                             vm->cpu.p |= XM65_FLAG_Z;
    if (R & 0x80)                                           vm->cpu.p |= XM65_FLAG_N;
    if (((~(vm->cpu.a ^ M) & (vm->cpu.a ^ R)) & 0x80) != 0) vm->cpu.p |= XM65_FLAG_V;
}

uint16_t XM65_ReadVector(XM65_VM *vm, uint16_t lo) {
    uint16_t r = vm->ram.data[lo] | (vm->ram.data[lo + 1] << 8);

    printf("* XM65_ReadVector: Reading from %u - +1 (%X %X)\n", (unsigned int) lo, vm->ram.data[lo+1], vm->ram.data[lo]);

    return r;
}

int XM65_ProgramVM(XM65_VM *vm, const char *filename) {
    char *file = NULL; long filesize;
    XM65_OpenFile(filename, &file, &filesize);

    // Copying PRG memory
    int i = 0;
    int interrupt_start = filesize - 6 /* vector region size */;

    printf("* XM65_ProgramVM: Filesize: %lu\n", filesize);
    printf("* XM65_ProgramVM: Interrupt Start: %i\n", interrupt_start);

    while (i < interrupt_start) {
        vm->ram.data[0x0200 + i] = file[i];
        i++;
    }

    // Copying VECTOR memory
    vm->ram.data[XM65_VECTOR_IRQ     ] = file[interrupt_start +0];
    vm->ram.data[XM65_VECTOR_IRQ   +1] = file[interrupt_start +1];
    vm->ram.data[XM65_VECTOR_RESET   ] = file[interrupt_start +2];
    vm->ram.data[XM65_VECTOR_RESET +1] = file[interrupt_start +3];
    vm->ram.data[XM65_VECTOR_NMI     ] = file[interrupt_start +4];
    vm->ram.data[XM65_VECTOR_NMI   +1] = file[interrupt_start +5];

    fputs("PRG: ", stdout);
    for (int i = 0; i < interrupt_start; i++) {
        printf("%X ", vm->ram.data[0x0200 + i]);
    }

    fputs("\nVECTOR: ", stdout);
    for (int i = 0; i < 6 /* vector region size */; i++) {
        printf("%X ", vm->ram.data[XM65_VECTOR_IRQ + i]);
    }

    fputs("\n", stdout);

    XM65_CloseFile(file);

    return 0;
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
    printf("* XM65_ResetVM: %u\n", (unsigned int) vm->cpu.pc);

    // Set interrupt disable flag: disable IRQ's, then continue
    vm->cpu.p |= XM65_FLAG_U | XM65_FLAG_B;

    // Reset cpu cycles counter
    vm->cpu.cycles = 6;
}

