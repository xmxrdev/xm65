#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "vm.h"
#include "utils.h"

void XM65_PrintCPU(XM65_VM *vm) {
    printf("A=$%X X=$%X Y=$%X\nSP=$%X PC=$%X\nNV-BDIZC\n%s\n", vm->cpu.a, vm->cpu.x, vm->cpu.y, vm->cpu.sp, vm->cpu.pc, XM65_ByteToBin(vm->cpu.p));
}

void XM65_ResetVM(XM65_VM *vm) {
    /// Simulating undefined state of CPU registers and RAM contents
    // Randomize CPU registers
    vm->cpu.a  = (uint8_t)(rand() & 0xFF);
    vm->cpu.x  = (uint8_t)(rand() & 0xFF);
    vm->cpu.y  = (uint8_t)(rand() & 0xFF);
    vm->cpu.p  = (uint8_t)(rand() & 0xFF);
    vm->cpu.sp = (uint8_t)(rand() & 0xFF);

    // Set interrupt disable flag: disable IRQ's
    vm->cpu.sp |= XM65_FLAG_I;

    // Randomize RAM contents
    for (size_t i = 0; i < RAM_SIZE; i++) {
        vm->ram.data[i] = (uint8_t)(rand() % 0xFF);
    }
}

void XM65_UpdateFlags(XM65_VM *vm, uint8_t M, uint8_t R, uint16_t RV) {
    vm->cpu.p &= ~(XM65_FLAG_C | XM65_FLAG_Z | XM65_FLAG_V | XM65_FLAG_N);

    if (RV > 0xFF)                                          vm->cpu.p |= XM65_FLAG_C;
    if (R == 0)                                             vm->cpu.p |= XM65_FLAG_Z;
    if (R & 0x80)                                           vm->cpu.p |= XM65_FLAG_N;
    if (((~(vm->cpu.a ^ M) & (vm->cpu.a ^ R)) & 0x80) != 0) vm->cpu.p |= XM65_FLAG_V;
}

void XM65_RunVM(XM65_VM *vm) {
    uint8_t  fetch = vm->ram.data[vm->cpu.pc++];
    uint8_t  M, R;
    uint16_t RV;

    switch (fetch) {
        case 0xEA: {
            break;
        }
        case 0x69: {
            M = vm->ram.data[vm->cpu.pc++];
            RV = vm->cpu.a + M + ((vm->cpu.p & XM65_FLAG_C) ? 1 : 0);
            R = (uint8_t) RV;

            // Update flags
            XM65_UpdateFlags(vm, M, R, RV);

            vm->cpu.a = R;

            // Track CPU cycles
            vm->cpu.cycles += 2;
            break;
        }
        default:
            break;
    }
}
