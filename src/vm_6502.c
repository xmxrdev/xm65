#include "xm65/vm.h"

XM65_VM_STATUS XM65_RunVM(XM65_VM *vm) {
    if (vm->status == XM65_VM_STATUS_INTERRUPTED) return vm->status;

    if (vm->status == XM65_VM_STATUS_IDLE) vm->status = XM65_VM_STATUS_RUNNING;

    uint16_t M = 0;
    uint16_t R = 0;
    uint8_t fetch = vm->ram.data[vm->cpu.pc++];
    XM65_FLAG flags = XM65_FLAGS_NONE;

    switch (fetch) {
        case XM65_OPCODE_ADC_IMM: { flags = XM65_FLAGS_ALL; M = XM65_ReadOperand(vm); R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_ADC_ZPG: { flags = XM65_FLAGS_ALL; M = XM65_ReadOperand(vm); M = vm->ram.data[M]; R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_ADC_ZPG_X: { flags = XM65_FLAGS_ALL; M = XM65_ReadOperand(vm); M = vm->ram.data[M + vm->cpu.x]; R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ADC_ABS: { flags = XM65_FLAGS_ALL; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ADC_ABS_Y: { flags = XM65_FLAGS_ALL; M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE); R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ADC_ABS_X: { flags = XM65_FLAGS_ALL; M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE); R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ADC_IND_X: { flags = XM65_FLAGS_ALL; M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE); R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_ADC_IND_Y: { flags = XM65_FLAGS_ALL; M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE); R = XM65_AddCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 5; break; }

        case XM65_OPCODE_AND_IMM: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a &= M; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_AND_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a &= vm->ram.data[M]; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_AND_ZPG_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a &= vm->ram.data[M + vm->cpu.x]; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_AND_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); vm->cpu.a &= M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_AND_ABS_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE); vm->cpu.a &= M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_AND_ABS_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE); vm->cpu.a &= M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_AND_IND_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset( vm, vm->cpu.x, 0, DEREFERENCE); vm->cpu.a &= M; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_AND_IND_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset( vm, 0, vm->cpu.y, DEREFERENCE); vm->cpu.a &= M; vm->cpu.cycles += 5; break; }

        case XM65_OPCODE_ASL_A: { flags = XM65_FLAGS_ZN; vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | (vm->cpu.a >> 7); vm->cpu.a <<= 1; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_ASL_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] >> 7); vm->ram.data[M] <<= 1; vm->cpu.cycles += 5; break; }
        case XM65_OPCODE_ASL_ZPG_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[(M + vm->cpu.x) & 0xFF] >> 7); vm->ram.data[(M + vm->cpu.x) & 0xFF] <<= 1; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_ASL_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] >> 7); vm->ram.data[M] <<= 1; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_ASL_ABS_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] >> 7); vm->ram.data[M] <<= 1; vm->cpu.cycles += 7; break; }

        case XM65_OPCODE_BRK_IMP: { vm->cpu.pc += 1; XM65_StackPush(vm, (uint8_t)(vm->cpu.pc & 0xFF00) >> 8); XM65_StackPush(vm, (uint8_t)(vm->cpu.pc & 0x00FF)); XM65_StackPush(vm, (vm->cpu.p | XM65_FLAG_B | XM65_FLAG_U)); vm->cpu.pc = XM65_ReadVector(vm, XM65_VECTOR_IRQ); vm->cpu.cycles += 7; vm->status = XM65_VM_STATUS_INTERRUPTED; break; }

        case XM65_OPCODE_CLC_IMP: { vm->cpu.p &= (uint8_t) ~(XM65_FLAG_C); vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_CLD_IMP: { vm->cpu.p &= (uint8_t) ~(XM65_FLAG_D); vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_CLI_IMP: { vm->cpu.p &= (uint8_t) ~(XM65_FLAG_I); vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_EOR_IMM: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a ^= M; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_EOR_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a ^= vm->ram.data[M]; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_EOR_ZPG_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a ^= vm->ram.data[M + vm->cpu.x]; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_EOR_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); vm->cpu.a ^= M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_EOR_ABS_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE); vm->cpu.a ^= M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_EOR_ABS_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE); vm->cpu.a ^= M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_EOR_IND_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE); vm->cpu.a ^= M; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_EOR_IND_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE); vm->cpu.a ^= M; vm->cpu.cycles += 5; break; }

        case XM65_OPCODE_INX_IMP: { flags = XM65_FLAGS_ZN; vm->cpu.x++; R = vm->cpu.x; vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_JMP_ABS: { vm->cpu.pc = XM65_ReadVector(vm, vm->cpu.pc); vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_JMP_IND: { vm->cpu.pc = XM65_ReadIndirectAbsolute(vm, NO_DEREFERENCE); vm->cpu.cycles += 5; break; }

        case XM65_OPCODE_JSR_ABS: { M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); XM65_StackPush(vm, (uint8_t)((vm->cpu.pc - 1) >> 8) & 0xFF); XM65_StackPush(vm, (uint8_t)((vm->cpu.pc - 1)       & 0xFF)); vm->cpu.pc = M; vm->cpu.cycles += 6; break; }

        case XM65_OPCODE_LDA_IMM: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a = (uint8_t)M; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_LDA_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); M = vm->ram.data[M]; vm->cpu.a = (uint8_t) M; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_LDA_ZPG_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); M = vm->ram.data[M + vm->cpu.x]; vm->cpu.a = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDA_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); vm->cpu.a = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDA_ABS_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE); vm->cpu.a = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDA_ABS_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE); vm->cpu.a = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDA_IND_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE); vm->cpu.a = (uint8_t)M; vm->cpu.cycles += 5; break; }
        case XM65_OPCODE_LDA_IND_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE); vm->cpu.a = (uint8_t)M; vm->cpu.cycles += 5; break; }

        case XM65_OPCODE_LDX_IMM: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); R = M; vm->cpu.x = (uint8_t)M; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_LDX_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); R = M; M = vm->ram.data[M]; vm->cpu.x = (uint8_t)M; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_LDX_ZPG_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); R = M; M = vm->ram.data[M + vm->cpu.y]; vm->cpu.x = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDX_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); R = M; vm->cpu.x = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDX_ABS_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE); R = M; vm->cpu.x = (uint8_t)M; vm->cpu.cycles += 4; break; }

        case XM65_OPCODE_LDY_IMM: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); R = M; vm->cpu.y = (uint8_t)M; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_LDY_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); R = M; M = vm->ram.data[M]; vm->cpu.y = (uint8_t)M; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_LDY_ZPG_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); R = M; M = vm->ram.data[M + vm->cpu.y]; vm->cpu.y = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDY_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); R = M; vm->cpu.y = (uint8_t)M; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_LDY_ABS_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE); R = M; vm->cpu.y = (uint8_t)M; vm->cpu.cycles += 4; break; }

        case XM65_OPCODE_LSR_IMM: { flags = XM65_FLAGS_ZN; vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | (vm->cpu.a & 1); vm->cpu.a >>= 1; R = vm->cpu.a; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_LSR_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] & 1); vm->ram.data[M] >>= 1; R = vm->ram.data[M]; vm->cpu.cycles += 5; break; }
        case XM65_OPCODE_LSR_ZPG_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[(M + vm->cpu.x) & 0xFF] & 1); vm->ram.data[(M + vm->cpu.x) & 0xFF] >>= 1; R = vm->ram.data[M]; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_LSR_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] & 1); vm->ram.data[M] >>= 1; R = vm->ram.data[M]; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_LSR_ABS_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE); vm->cpu.p = (vm->cpu.p & ~(XM65_FLAG_C)) | ((uint8_t) vm->ram.data[M] & 1); vm->ram.data[M] >>= 1; R = vm->ram.data[M]; vm->cpu.cycles += 7; break; }

        case XM65_OPCODE_NOP_IMP: { vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_ORA_IMM: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a |= M; R = vm->cpu.a; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_ORA_ZPG: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a |= vm->ram.data[M]; R = vm->cpu.a; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_ORA_ZPG_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadOperand(vm); vm->cpu.a |= vm->ram.data[M + vm->cpu.x]; R = vm->cpu.a; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ORA_ABS: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); vm->cpu.a |= M; R = vm->cpu.a; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ORA_ABS_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE); vm->cpu.a |= M; R = vm->cpu.a; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ORA_ABS_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE); vm->cpu.a |= M; R = vm->cpu.a; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_ORA_IND_X: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE); vm->cpu.a |= M; R = vm->cpu.a; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_ORA_IND_Y: { flags = XM65_FLAGS_ZN; M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE); vm->cpu.a |= M; R = vm->cpu.a; vm->cpu.cycles += 5; break; }

        case XM65_OPCODE_RTI_IMP: { vm->cpu.p = XM65_StackPull(vm) | XM65_FLAG_U; vm->cpu.pc  = XM65_StackPull(vm); vm->cpu.pc |= XM65_StackPull(vm) << 8; vm->cpu.cycles += 6; break; }

        case XM65_OPCODE_SEC_IMP: { vm->cpu.p |= XM65_FLAG_C; vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_SEI_IMP: { vm->cpu.p |= XM65_FLAG_I; vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_TXS_IMP: { flags = XM65_FLAGS_ZN; vm->cpu.sp = vm->cpu.x; R = vm->cpu.x; vm->cpu.cycles += 2; break; }

        case XM65_OPCODE_STA_ZPG: { M = XM65_ReadOperand(vm); vm->ram.data[M] = vm->cpu.a; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_STA_ZPG_X: { M = XM65_ReadOperand(vm); vm->ram.data[M + vm->cpu.x] = vm->cpu.a; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_STA_ABS: { M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); vm->ram.data[M] = vm->cpu.a; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_STA_ABS_Y: { M = XM65_ReadAbsolute(vm, vm->cpu.y, NO_DEREFERENCE); vm->ram.data[M] = vm->cpu.a; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_STA_ABS_X: { M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE); vm->ram.data[M] = vm->cpu.a; vm->cpu.cycles += 5; break; }
        case XM65_OPCODE_STA_IND_X: { M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, NO_DEREFERENCE); vm->ram.data[M] = vm->cpu.a; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_STA_IND_Y: { M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, NO_DEREFERENCE); vm->ram.data[M] = vm->cpu.a; vm->cpu.cycles += 6; break; }

        case XM65_OPCODE_STX_ZPG: { M = XM65_ReadOperand(vm); vm->ram.data[M] = vm->cpu.x; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_STX_ZPG_Y: { M = XM65_ReadOperand(vm); vm->ram.data[M + vm->cpu.y] = vm->cpu.x; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_STX_ABS: { M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); vm->ram.data[M] = vm->cpu.x; vm->cpu.cycles += 4; break; }

        case XM65_OPCODE_STY_ZPG: { M = XM65_ReadOperand(vm); vm->ram.data[M] = vm->cpu.y; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_STY_ZPG_X: { M = XM65_ReadOperand(vm); vm->ram.data[M + vm->cpu.x] = vm->cpu.y; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_STY_ABS: { M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); vm->ram.data[M] = vm->cpu.y; vm->cpu.cycles += 4; break; }

        case XM65_OPCODE_SBC_IMM: { flags = XM65_FLAGS_ALL; M = XM65_ReadOperand(vm); R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 2; break; }
        case XM65_OPCODE_SBC_ZPG: { flags = XM65_FLAGS_ALL; M = XM65_ReadOperand(vm); M = vm->ram.data[M]; R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 3; break; }
        case XM65_OPCODE_SBC_ZPG_X: { flags = XM65_FLAGS_ALL; M = XM65_ReadOperand(vm); M = vm->ram.data[M + vm->cpu.x]; R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_SBC_ABS: { flags = XM65_FLAGS_ALL; M = XM65_ReadAbsolute(vm, 0, DEREFERENCE); R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_SBC_ABS_Y: { flags = XM65_FLAGS_ALL; M = XM65_ReadAbsolute(vm, vm->cpu.y, DEREFERENCE); R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_SBC_ABS_X: { flags = XM65_FLAGS_ALL; M = XM65_ReadAbsolute(vm, vm->cpu.x, DEREFERENCE); R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 4; break; }
        case XM65_OPCODE_SBC_IND_X: { flags = XM65_FLAGS_ALL; M = XM65_ReadIndirectOffset(vm, vm->cpu.x, 0, DEREFERENCE); R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_SBC_IND_Y: { flags = XM65_FLAGS_ALL; M = XM65_ReadIndirectOffset(vm, 0, vm->cpu.y, DEREFERENCE); R = XM65_SubstractCarry(vm, (uint8_t)M); vm->cpu.a = (uint8_t)R; vm->cpu.cycles += 5; break; }

        case XM65_OPCODE_INC_ZPG: { M = XM65_ReadOperand(vm); vm->ram.data[M]++; R = vm->ram.data[M]; vm->cpu.cycles += 5; break; }
        case XM65_OPCODE_INC_ZPG_X: { M = XM65_ReadOperand(vm); vm->ram.data[M + vm->cpu.x]++; R = vm->ram.data[M + vm->cpu.x]; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_INC_ABS: { M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); vm->ram.data[M]++; R = vm->ram.data[M]; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_INC_ABS_X: { M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE); vm->ram.data[M]++; R = vm->ram.data[M]; vm->cpu.cycles += 7; break; }

        case XM65_OPCODE_DEC_ZPG: { M = XM65_ReadOperand(vm); vm->ram.data[M]--; R = vm->ram.data[M]; vm->cpu.cycles += 5; break; }
        case XM65_OPCODE_DEC_ZPG_X: { M = XM65_ReadOperand(vm); vm->ram.data[M + vm->cpu.x]--; R = vm->ram.data[M + vm->cpu.x]; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_DEC_ABS: { M = XM65_ReadAbsolute(vm, 0, NO_DEREFERENCE); vm->ram.data[M]--; R = vm->ram.data[M]; vm->cpu.cycles += 6; break; }
        case XM65_OPCODE_DEC_ABS_X: { M = XM65_ReadAbsolute(vm, vm->cpu.x, NO_DEREFERENCE); vm->ram.data[M]--; R = vm->ram.data[M]; vm->cpu.cycles += 7; break; }
    }

    XM65_UpdateFlags(vm, M, R, flags);

    return vm->status;
}
