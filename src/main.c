#include <stdio.h>

#include "cli.h"
#include "vm.h"

int main(int argc, char *argv[]) {
    XM65_Cli cli;
    XM65_ParseArguments(argc, argv, &cli);

    if (cli.target == NULL) XM65_ThrowError(XM65_ERROR_NO_TARGETS);

    printf("INFO: Target file: %s\n", cli.target);
    printf("INFO: Flags: %zu\n", (size_t) cli.flags);


    // TESTING & DEBUGGING
    XM65_VM vm;
    XM65_Power_VM(&vm);
    if (XM65_ProgramVM(&vm, cli.target) != 0) return 1;
    XM65_ResetVM(&vm);

    // INFO: Temporal for debugging
    // Print program memory region (0x0200-0x0400)
    printf("Program Memory (0x0200-0x0400):\n");
    for (int i = 0x0200; i < 0x0400; i += 16) {
        printf("%04X: ", i);
        for (int j = 0; j < 16; j++) {
            printf("%02X ", vm.ram.data[i + j]);
        }
        printf("\n");
    }

    // Print last page with vectors (0xFF00-0x10000)
    printf("\nLast Page with Vectors (0xFF00-0x10000):\n");
    for (int i = 0xFF00; i < 0x10000; i += 16) {
        printf("%04X: ", i);
        for (int j = 0; j < 16; j++) {
            printf("%02X ", vm.ram.data[i + j]);
        }
        printf("\n");
    }

    while ((vm.status != XM65_VM_STATUS_INTERRUPTED)) {
        XM65_RunVM(&vm); XM65_PrintCPU(&vm);
    }

    return 0;
}
