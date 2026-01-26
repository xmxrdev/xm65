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
        case 0x01: {
                       M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE);

                       vm->cpu.a |= M;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x05: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a |= vm->ram.data[M];

                       vm->cpu.cycles += 3;

                       break;
                   }
        case 0x06: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] >> 7);
                       vm->ram.data[M] <<= 1;

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0x09: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a |= M;

                       vm->cpu.cycles += 2;

                       break;
                   }
        case 0x0A: {
                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | (vm->cpu.a >> 7);
                       vm->cpu.a <<= 1;

                       vm->cpu.cycles += 2;

                       break;
                   }
        case 0x0D: {
                       M = XM65_ReadAbsolute(vm, 0, DEREFERENCE);

                       vm->cpu.a |= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x0E: {
                       M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] >> 7);
                       vm->ram.data[M] <<= 1;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x11: {
                       M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE);

                       vm->cpu.a |= M;

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0x15: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a |= vm->ram.data[M + vm->cpu.x];

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x16: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[(M + vm->cpu.x) & 0xFF] >> 7);
                       vm->ram.data[(M + vm->cpu.x) & 0xFF] <<= 1;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x18: {
                       vm->cpu.p &= (uint8_t) ~(XM65_FLAG_C);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x19: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE);

                       vm->cpu.a |= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x1D: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE);

                       vm->cpu.a |= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x1E: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] >> 7);
                       vm->ram.data[M] <<= 1;

                       vm->cpu.cycles += 7;

                       break;
                   }
        case 0x20: {
                       M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE);

                       XM65_StackPush(vm, (uint8_t)((vm->cpu.pc - 1) >> 8) & 0xFF);
                       XM65_StackPush(vm, (uint8_t)((vm->cpu.pc - 1)       & 0xFF));

                       vm->cpu.pc = M;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x21: {
                       M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE);

                       vm->cpu.a &= M;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x25: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a &= vm->ram.data[M];

                       vm->cpu.cycles += 3;

                       break;
                   }
        case 0x29: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a &= M;

                       vm->cpu.cycles += 2;

                       break;
                   }
        case 0x2D: {
                       M = XM65_ReadAbsolute(vm, 0, DEREFERENCE);

                       vm->cpu.a &= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x31: {
                       M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE);

                       vm->cpu.a &= M;

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0x35: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a &= vm->ram.data[M + vm->cpu.x];

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x38: {
                       vm->cpu.p |= XM65_FLAG_C;

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x39: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE);

                       vm->cpu.a &= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x3D: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE);

                       vm->cpu.a &= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x40: {
                       vm->cpu.p   = XM65_StackPull(vm) | XM65_FLAG_U;
                       vm->cpu.pc  = XM65_StackPull(vm);
                       vm->cpu.pc |= XM65_StackPull(vm) << 8;

                       vm->cpu.cycles += 6;
                       break;
                   }
        case 0x41: {
                       M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE);

                       vm->cpu.a ^= M;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x45: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a ^= vm->ram.data[M];

                       vm->cpu.cycles += 3;

                       break;
                   }
        case 0x46: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] & 1);
                       vm->ram.data[M] >>= 1;

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0x49: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a ^= M;

                       vm->cpu.cycles += 2;

                       break;
                   }
        case 0x4A: {
                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | (vm->cpu.a & 1);
                       vm->cpu.a >>= 1;

                       vm->cpu.cycles += 2;

                       break;
                   }
        case 0x4C: {
                       vm->cpu.pc = XM65_ReadVector(vm, vm->cpu.pc);
           
                       vm->cpu.cycles += 3;
                       break;
                   }
        case 0x4D: {
                       M = XM65_ReadAbsolute(vm, 0, DEREFERENCE);

                       vm->cpu.a ^= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x4E: {
                       M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] & 1);
                       vm->ram.data[M] >>= 1;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x51: {
                       M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE);

                       vm->cpu.a ^= M;

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0x55: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.a ^= vm->ram.data[M + vm->cpu.x];

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x56: {
                       M = XM65_ReadOperand(vm);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[(M + vm->cpu.x) & 0xFF] & 1);
                       vm->ram.data[(M + vm->cpu.x) & 0xFF] >>= 1;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x58: {
                       vm->cpu.p &= (uint8_t) ~(XM65_FLAG_I);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x59: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE);

                       vm->cpu.a ^= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x5D: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE);

                       vm->cpu.a ^= M;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x5E: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE);

                       vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] & 1);
                       vm->ram.data[M] >>= 1;

                       vm->cpu.cycles += 7;

                       break;
                   }
        case 0x61: {
                       M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE);

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
        case 0x6C: {
                       vm->cpu.pc = XM65_ReadIndirectAbsolute(vm, NO_DEREFERENCE);

                       vm->cpu.cycles += 5;

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
                       M  = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE);

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
        case 0x81: {
                       M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.a;

                       vm->cpu.cycles += 6;

                       break;
                   }
        case 0x84: {
                       M = XM65_ReadOperand(vm);

                       vm->ram.data[M] = vm->cpu.y;

                       vm->cpu.cycles += 3;

                       break;
                   }
        case 0x85: {
                       M = XM65_ReadOperand(vm);

                       vm->ram.data[M] = vm->cpu.a;

                       vm->cpu.cycles += 3;

                       break;
                   }
        case 0x86: {
                       M = XM65_ReadOperand(vm);

                       vm->ram.data[M] = vm->cpu.x;

                       vm->cpu.cycles += 3;

                       break;
                   }
        case 0x8C: {
                       M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.y;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x8D: {
                       M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.a;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x8E: {
                       M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.x;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x91: {
                       M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.a;

                       vm->cpu.cycles += 6;

                       break;
                   }

        case 0x94: {
                       M = XM65_ReadOperand(vm);

                       vm->ram.data[M + vm->cpu.x] = vm->cpu.y;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x95: {
                       M = XM65_ReadOperand(vm);

                       vm->ram.data[M + vm->cpu.x] = vm->cpu.a;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x96: {
                       M = XM65_ReadOperand(vm);

                       vm->ram.data[M + vm->cpu.y] = vm->cpu.x;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0x9A: {
                       vm->cpu.sp = vm->cpu.x;

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0x9D: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.a;

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0x99: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.y, NO_DEREFERENCE);

                       vm->ram.data[M] = vm->cpu.a;

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xA0: {
                       M = XM65_ReadOperand(vm);
                       vm->cpu.y = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 2;

                       break;
                   }
        case 0xA1: {
                       M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE);

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
        case 0xA4: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M];

                       vm->cpu.y = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 3;

                       break;
                   }
        case 0xA5: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M];

                       vm->cpu.a = (uint8_t) M;

                       vm->cpu.cycles += 3;
                       break;
                   }
        case 0xA6: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M];

                       vm->cpu.x = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

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
        case 0xAE: {
                       M = XM65_ReadAbsolute(vm, 0, DEREFERENCE);

                       vm->cpu.x = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xAC: {
                       M = XM65_ReadAbsolute(vm, 0, DEREFERENCE);

                       vm->cpu.y = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xB1: {
                       M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE);

                       vm->cpu.a = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 5;

                       break;
                   }
        case 0xB4: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M + vm->cpu.y];

                       vm->cpu.y = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

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
        case 0xB6: {
                       M = XM65_ReadOperand(vm);
                       M = vm->ram.data[M + vm->cpu.y];

                       vm->cpu.x = (uint8_t) M;

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
        case 0xBC: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE);

                       vm->cpu.y = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xBE: {
                       M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE);

                       vm->cpu.x = (uint8_t) M;

                       XM65_UpdateFlags(vm, M, (uint8_t) M, RV, XM65_FLAG_Z | XM65_FLAG_N);

                       vm->cpu.cycles += 4;

                       break;
                   }
        case 0xD8: {
                       vm->cpu.p &= (uint8_t) ~(XM65_FLAG_D);

                       vm->cpu.cycles += 2;
                       break;
                   }
        case 0xE8: {
                       vm->cpu.x++;
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
