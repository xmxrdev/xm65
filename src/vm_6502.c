#include "vm.h"

XM65_VM_STATUS XM65_RunVM(XM65_VM *vm) {
    if (vm->status == XM65_VM_STATUS_INTERRUPTED) return vm->status;

    uint8_t  fetch = vm->ram.data[vm->cpu.pc++]; // +1 cycle (fetch)
    uint8_t  M, R;
    uint16_t RV = 0;

    if (vm->status == XM65_VM_STATUS_IDLE) vm->status = XM65_VM_STATUS_RUNNING;

    switch (fetch) {
        case 0x00: {
                       vm->cpu.pc += 1; // +1 cycle
                       XM65_StackPush(vm, (vm->cpu.pc & 0xFF00) >> 8); // +1 cycle
                       XM65_StackPush(vm, (vm->cpu.pc & 0x00FF)); // +1 cycle
                       XM65_StackPush(vm, (vm->cpu.p | XM65_FLAG_B | XM65_FLAG_U)); // +1 cycle
                       // vm->cpu.p |= XM65_FLAG_I; // (65C02)
                       vm->cpu.pc = XM65_ReadVector(vm, XM65_VECTOR_IRQ); // +2 cycles
                       
                       vm->cpu.cycles += 7;

                       vm->status = XM65_VM_STATUS_INTERRUPTED;
                       break;
                   }
        case 0x18: {
                       vm->cpu.p &= ~(XM65_FLAG_C); // +1 cycles

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x38: {
                       vm->cpu.p |= XM65_FLAG_C; // +1 cycles

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x40: {
                       vm->cpu.p   = XM65_StackPull(vm) | XM65_FLAG_U;
                       vm->cpu.pc  = XM65_StackPull(vm);
                       vm->cpu.pc |= XM65_StackPull(vm) << 8;

                       vm->cpu.cycles += 6;
                       break;
                   }
        case 0x4C: {
                       vm->cpu.pc = XM65_ReadVector(vm, vm->cpu.pc); // +2 cycles
           
                       vm->cpu.cycles += 3;
                       break;
                   }
        case 0x58: {
                       vm->cpu.p &= ~(XM65_FLAG_I); // +1 cycles

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x69: {
                       M = vm->ram.data[vm->cpu.pc++]; // +1 cycle
                       RV = vm->cpu.a + M + ((vm->cpu.p & XM65_FLAG_C) ? 1 : 0);
                       R = (uint8_t) RV;
           
                       XM65_UpdateFlags(vm, M, R, RV);
           
                       vm->cpu.a = R;
           
                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x78: {
                       vm->cpu.p |= XM65_FLAG_I; // +1 cycles

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x9A: {
                       vm->cpu.sp = vm->cpu.x; // +1 cycles

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xEA: {
                       vm->cpu.cycles += 2; // +1 cycles
                       break;
                   }
        case 0xD8: {
                       vm->cpu.p &= ~(XM65_FLAG_D); // +1 cycles

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xA2: {
                       M = vm->ram.data[vm->cpu.pc++]; // +1 cycle
                       vm->cpu.x = M;
           
                       XM65_UpdateFlags(vm, M, M, RV);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xA9: {
                       M = vm->ram.data[vm->cpu.pc++]; // +1 cycle
                       vm->cpu.a = M;
           
                       XM65_UpdateFlags(vm, M, M, RV);
           
                       vm->cpu.cycles += 2;
                       break;
                   }
        default: {
                     break;
                 }
    }

    return vm->status;
}
