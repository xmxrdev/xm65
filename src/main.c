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
    if (XM65_ProgramVM(&vm, cli.target) != 0) return 1;
    XM65_ResetVM(&vm);

    vm.cpu.pc = 0x200; // Just to avoid RESET routine

    while ((vm.status != XM65_VM_STATUS_INTERRUPTED)) {
        XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    }

    return 0;
}
