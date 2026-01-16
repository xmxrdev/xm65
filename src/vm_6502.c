#include "vm.h"

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
            vm->cpu.pc = XM65_ReadVector(vm, XM65_VECTOR_IRQ); // +2 cycles
            
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

            XM65_UpdateFlags(vm, M, R, RV);

            vm->cpu.a = R;

            vm->cpu.cycles += 1;
            break;
        }
        case 0xA9: {
            M = vm->ram.data[vm->cpu.pc++]; // +1 cycle
            vm->cpu.a = M;

            XM65_UpdateFlags(vm, M, M, M);

            break;
        }
        default: {
            break;
        }
    }

    vm->cpu.cycles++;
}
