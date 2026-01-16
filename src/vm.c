#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "vm.h"
#include "utils.h"

void XM65_PrintCPU(XM65_VM *vm) {
    printf("A=$%X X=$%X Y=$%X\nSP=$%X PC=$%X\nNV-BDIZC\n%s (0x%X)\n\n", vm->cpu.a, vm->cpu.x, vm->cpu.y, vm->cpu.sp, vm->cpu.pc, XM65_ByteToBin(vm->cpu.p), vm->cpu.p);
}

void XM65_UpdateFlags(XM65_VM *vm, uint8_t M, uint8_t R, uint16_t RV) {
    vm->cpu.p &= ~(XM65_FLAG_C | XM65_FLAG_Z | XM65_FLAG_V | XM65_FLAG_N);

    if (RV > 0xFF)                                          vm->cpu.p |= XM65_FLAG_C;
    if (R == 0)                                             vm->cpu.p |= XM65_FLAG_Z;
    if (R & 0x80)                                           vm->cpu.p |= XM65_FLAG_N;
    if (((~(vm->cpu.a ^ M) & (vm->cpu.a ^ R)) & 0x80) != 0) vm->cpu.p |= XM65_FLAG_V;
}

uint16_t XM65_ReadVector(XM65_VM *vm, uint16_t lo) {
    return vm->ram.data[lo] | (vm->ram.data[lo + 1] << 8);
}

int XM65_ProgramVM(XM65_VM *vm, const char *filename) {
    char *file = NULL; long filesize;
    XM65_OpenFile(filename, &file, &filesize);
    
    for (int i = 0; i < filesize - (2 * 3); i++) {
        printf("%X ", (uint8_t) file[i]);
    }

    puts("");

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

    // Set interrupt disable flag: disable IRQ's, then continue
    vm->cpu.p |= XM65_FLAG_U | XM65_FLAG_B;

    // Reset cpu cycles counter
    vm->cpu.cycles = 6;
}

void XM65_RunVM(XM65_VM *vm) {
    uint8_t  fetch = vm->ram.data[vm->cpu.pc++]; // +1 cycle (fetch)
    uint8_t  M, R;
    uint16_t RV;

    switch (fetch) {
        case 0x00: {
            vm->cpu.pc += 1; // +1 cycle
            vm->ram.data[0x0100 | vm->cpu.sp--] = (vm->cpu.pc & 0xFF00) >> 8; // +1 cycle
            vm->ram.data[0x0100 | vm->cpu.sp--] = (vm->cpu.pc & 0x00FF); // +1 cycle
            vm->ram.data[0x0100 | vm->cpu.sp--] = (vm->cpu.p | XM65_FLAG_B | XM65_FLAG_U); // +1 cycle
            // vm->cpu.p |= XM65_FLAG_I; // (65C02)
            vm->cpu.pc = XM65_ReadVector(vm, XM65_VECTOR_INTERRUPT); // +2 cycles
            
            vm->cpu.cycles += 6;
            break;
        }
        case 0x18: {
            vm->cpu.p &= ~(XM65_FLAG_C); // +1 cycles
            vm->cpu.cycles += 1;
            break;
        }
        case 0x38: {
            vm->cpu.p |= XM65_FLAG_C; // +1 cycles
            vm->cpu.cycles += 1;
            break;
        }
        case 0xEA: {
            vm->cpu.cycles += 1; // +1 cycles
            break;
        }
        case 0x69: {
            M = vm->ram.data[vm->cpu.pc++]; // +1 cycle
            RV = vm->cpu.a + M + ((vm->cpu.p & XM65_FLAG_C) ? 1 : 0);
            R = (uint8_t) RV;

            // Update flags
            XM65_UpdateFlags(vm, M, R, RV);

            vm->cpu.a = R;

            vm->cpu.cycles += 1;
            break;
        }
        default:
            break;
    }

    vm->cpu.cycles++;
}
