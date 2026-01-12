#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "cli.h"

const char *options[] = {
    "--emulate",
    "--debug", "--assemble",
    "--config",
    "--version",
    "--help",
    NULL,
};

const char *error_strings[] = {
    "Target undefined. Please select a `.asm` or `.bin` file.",
    "No target files.",
};

bool str_ends_with(const char *str, const char *suffix) {
    if (!str || !suffix)
        return false;

    size_t len_str = strlen(str);
    size_t len_suf = strlen(suffix);

    if (len_suf > len_str)
        return false;

    return strcmp(str + len_str - len_suf, suffix) == 0;
}

int XM65_ThrowError(size_t code) {
    printf("ERROR: %s\n", error_strings[code]);
    printf("Use `xm65 --help E%zu` for more information.\n", code);
    return 1;
}

int XM65_ParseArguments(int argc, char *argv[], XM65_Cli *cli) {
    cli->target = NULL;
    cli->flags = XM65_CLI_NO_FLAGS;

    // Argument parser
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        
        if (arg[0] == '-') {
            size_t option = 0;
            const char* option_str = options[option];

            while (option_str != NULL) {
                int cmp = strcmp(option_str, arg);

                if (cmp == 0) cli->flags |= 1 << option;

                option_str = options[++option];
            }
        } else {
            bool target_asm = str_ends_with(arg, ".asm");
            bool target_bin = str_ends_with(arg, ".bin");

            if (target_asm || target_bin) {
                if (cli->flags & XM65_CLI_TARGET_CHOSEN) return XM65_ThrowError(0);

                cli->flags |= XM65_CLI_TARGET_CHOSEN;
                cli->target = arg;

                printf("Chosen target: %s\n", arg);
            }
        }
    }

    return 0;
}
