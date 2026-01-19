#include <stdio.h>

#include "xm65/cli.h"
#include "xm65/utils.h"
#include "xm65/vm.h"

int main(int argc, char *argv[]) {
    XM65_Cli cli;
    XM65_ParseArguments(argc, argv, &cli);

    if (cli.target == NULL) XM65_ThrowError(XM65_ERROR_NO_TARGETS);

    printf("INFO: Target file: %s\n", cli.target);
    printf("INFO: Flags: %zu\n", (size_t) cli.flags);

    if (cli.flags & XM65_CLI_EMULATE_TARGET) {
        if (!XM65_CanReadFile(cli.target)) XM65_ThrowError(XM65_ERROR_FILE_OPEN, cli.target);

        XM65_VM vm;
        XM65_Power_VM(&vm);
        XM65_ProgramVM(&vm, cli.target);
        XM65_ResetVM(&vm);

        while ((vm.status != XM65_VM_STATUS_INTERRUPTED)) {
            XM65_RunVM(&vm); XM65_PrintCPU(&vm);
        }
    }

    return 0;
}
