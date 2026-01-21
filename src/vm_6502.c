#include "xm65/vm.h"

XM65_VM_STATUS XM65_RunVM(XM65_VM *vm) {
    if (vm->status == XM65_VM_STATUS_INTERRUPTED) return vm->status;

    uint8_t  fetch = vm->ram.data[vm->cpu.pc++];
    uint16_t M;
    uint8_t  R;
    uint16_t RV = 0;

    if (vm->status == XM65_VM_STATUS_IDLE) vm->status = XM65_VM_STATUS_RUNNING;

    switch (fetch) {
        case 0x00: {
                       vm->cpu.pc += 1;
                       XM65_StackPush(vm, (uint8_t)(vm->cpu.pc & 0xFF00) >> 8);
                       XM65_StackPush(vm, (uint8_t)(vm->cpu.pc & 0x00FF));
                       XM65_StackPush(vm, (vm->cpu.p | XM65_FLAG_B | XM65_FLAG_U));
                       // vm->cpu.p |= XM65_FLAG_I; // (65C02)
                       vm->cpu.pc = XM65_ReadVector(vm, XM65_VECTOR_IRQ);
                       
                       vm->cpu.cycles += 7;

                       vm->status = XM65_VM_STATUS_INTERRUPTED;
                       break;
                   }
        case 0x18: {
                       vm->cpu.p &= (uint8_t) ~(XM65_FLAG_C);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x38: {
                       vm->cpu.p |= XM65_FLAG_C;

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
                       vm->cpu.pc = XM65_ReadVector(vm, vm->cpu.pc);
           
                       vm->cpu.cycles += 3;
                       break;
                   }
        case 0x58: {
                       vm->cpu.p &= (uint8_t) ~(XM65_FLAG_I);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x61: {
                       M = XM65_ReadIndirect(vm, vm->cpu.x, 0);

                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;

                       XM65_UpdateFlags(vm, M, R, RV, XM65_FLAG_ALL);

                       vm->cpu.a = R;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x65: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M];

                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;

                       vm->cpu.a = R;

                       vm->cpu.cycles += 3;
                       break;
                   }
        case 0x69: {
                       M = XM65_ReadOperand(vm);
                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;
           
                       XM65_UpdateFlags(vm, M, R, RV, XM65_FLAG_ALL);
           
                       vm->cpu.a = R;
           
                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x6D: {
                       M  = XM65_ReadOperand(vm);
                       M |= XM65_ReadOperand(vm) << 8;
                       M = vm->ram.data[M];

                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;

                       XM65_UpdateFlags(vm, M, R, RV, XM65_FLAG_ALL);

                       vm->cpu.a = R;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x71: {
                       M  = XM65_ReadIndirect(vm, 0, vm->cpu.y);

                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;

                       XM65_UpdateFlags(vm, M, R, RV, XM65_FLAG_ALL);

                       vm->cpu.a = R;

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0x75: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M + vm->cpu.x];

                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;

                       XM65_UpdateFlags(vm, M, R, RV, XM65_FLAG_ALL);

                       vm->cpu.a = R;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x78: {
                       vm->cpu.p |= XM65_FLAG_I;

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x79: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE);
                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;

                       XM65_UpdateFlags(vm, M, R, RV, XM65_FLAG_ALL);

                       vm->cpu.a = R;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x7D: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE);
                       RV = XM65_AddCarry(vm, (uint8_t) M);
                       R = (uint8_t) RV;

                       XM65_UpdateFlags(vm, M, R, RV, XM65_FLAG_ALL);

                       vm->cpu.a = R;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x8D: {
                       M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.a;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x9A: {
                       vm->cpu.sp = vm->cpu.x;

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xA1: {
                       M = XM65_ReadIndirect(vm, vm->cpu.x, 0);

                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0xA2: {
                       M = XM65_ReadOperand(vm);
                       vm->cpu.x = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xA5: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M];

                       vm->cpu.a = (uint8_t) M;

                       vm->cpu.cycles += 3;
                       break;
                   }
        case 0xA9: {
                       M = XM65_ReadOperand(vm);
                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xAD: {
                       M = XM65_ReadAbsolute(vm, 0, DEREFERENCE);

                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xB1: {
                       M = XM65_ReadIndirect(vm, 0, vm->cpu.y);

                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0xB5: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M + vm->cpu.x];

                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xB9: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE);
                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xBD: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE);
                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xD8: {
                       vm->cpu.p &= (uint8_t) ~(XM65_FLAG_D);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xEA: {
                       vm->cpu.cycles += 2;
                       break;
                   }
        default: {
                     break;
                 }
    }

    return vm->status;
}
