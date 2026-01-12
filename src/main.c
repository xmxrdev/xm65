#include <stdio.h>

#include "cli.h"

int main(int argc, char *argv[]) {
    XM65_Cli cli;
    XM65_ParseArguments(argc, argv, &cli);

    if (cli.target == NULL) return XM65_ThrowError(1);

    printf("INFO: Target file: %s\n", cli.target);
    printf("INFO: Flags: %zu\n", (size_t) cli.flags);

    return 0;
}
