#include <stdio.h>

#include "cli.h"
#include "vm.h"

int main(int argc, char *argv[]) {
    XM65_Cli cli;
    XM65_ParseArguments(argc, argv, &cli);

    if (cli.target == NULL) return XM65_ThrowError(1);

    printf("INFO: Target file: %s\n", cli.target);
    printf("INFO: Flags: %zu\n", (size_t) cli.flags);


    // TESTING & DEBUGGING
    XM65_VM vm;
    XM65_Power_VM(&vm);
    XM65_ProgramVM(&vm, cli.target);
    XM65_ResetVM(&vm);

    vm.cpu.pc = 0x200; // Just to avoid RESET routine

    XM65_PrintCPU(&vm);
    XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    XM65_RunVM(&vm); XM65_PrintCPU(&vm);

    //
    // // Vector settings
    // vm.ram.data[XM65_VECTOR_RESET    ] = 0x00;
    // vm.ram.data[XM65_VECTOR_RESET + 1] = 0x02;
    //
    // // Manual program allocation
    // vm.ram.data[0x200] = 0x18;                         // CLC
    // vm.ram.data[0x201] = 0x69; vm.ram.data[0x202] = 1; // ADC #$01 ; Expecting 0x01
    // vm.ram.data[0x203] = 0x38;                         // SEC
    // vm.ram.data[0x204] = 0x69; vm.ram.data[0x205] = 1; // ADC #$01 ; Expecting 0x03
    //
    // XM65_ResetVM(&vm);
    //
    // vm.cpu.a = 0;
    //
    // // Debugging
    // XM65_PrintCPU(&vm);
    // XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    // XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    // XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    // XM65_RunVM(&vm); XM65_PrintCPU(&vm);

    return 0;
}
