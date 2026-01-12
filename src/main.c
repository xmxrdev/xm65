#include <stdio.h>

#include "cli.h"

int main(int argc, char *argv[]) {
    parse_arguments(argc, argv);

    if (cli.target == NULL) return error(1);

    printf("INFO: Target file: %s\n", cli.target);
    printf("INFO: Flags: %zu\n", cli.flags);

    return 0;
}
