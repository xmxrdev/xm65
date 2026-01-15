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
    XM65_ResetVM(&vm);

    // Manual reset
    vm.cpu.p = XM65_FLAG_U | XM65_FLAG_B;
    vm.cpu.a = 0;
    vm.cpu.pc = 0x200;
    
    XM65_PrintCPU(&vm);

    // Manual program allocation
    vm.ram.data[0x200] = 0x69; vm.ram.data[0x201] = 1; // ADC #$01 ; Expecting 0x01
    vm.cpu.p |= XM65_FLAG_C;                           // SEC
    vm.ram.data[0x202] = 0x69; vm.ram.data[0x203] = 1; // ADC #$01 ; Expecting 0x03

    XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    XM65_RunVM(&vm); XM65_PrintCPU(&vm);

    return 0;
}
