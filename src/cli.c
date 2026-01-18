#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "cli.h"

const char *options[] = {
    "--emulate",
    "--config",
    "--version",
    "--help",
    NULL,
};

const char *error_strings[] = {
    "No target files.",
    "Target undefined. Please specify a `.bin` file.",
    "Failed to open file: '%s'.",
    "Failed to allocate memory.",
    "Failed to read file '%s' completely.",
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

void XM65_ThrowError(enum XM65_ERROR_CODE code, ...) {
    va_list args;

    fputs("ERROR: ", stderr);

    va_start(args, code);
    vfprintf(stderr, error_strings[code], args);
    va_end(args);

    fputs("\n", stderr);

    printf("Use `xm65 --help E%u` for more information.\n", code);
    exit(1);
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
            bool target_bin = str_ends_with(arg, ".bin");

            if (target_bin) {
                if (cli->flags & XM65_CLI_TARGET_CHOSEN) XM65_ThrowError(XM65_ERROR_TARGET_UNDEFINED);

                cli->flags |= XM65_CLI_TARGET_CHOSEN | XM65_CLI_EMULATE_TARGET;
                cli->target = arg;
            }
        }
    }

    return 0;
}
