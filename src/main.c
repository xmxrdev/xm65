#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

enum cli_flags {
    /* OPTIONS */
    CLI_NO_FLAGS           = 0x0000000000000000,
    CLI_EMULATE_TARGET     = 0x0000000000000001,
    CLI_EMULATE_WITH_DEBUG = 0x0000000000000002,
    CLI_ASSEMBLE_TARGET    = 0x0000000000000004,
    CLI_VM_CONFIG          = 0x0000000000000008,
    CLI_VERSION            = 0x0000000000000010,
    CLI_HELP               = 0x0000000000000020,

    CLI_RUN_TARGET         = CLI_EMULATE_TARGET | CLI_ASSEMBLE_TARGET,

    /* FLAGS*/
    CLI_TARGET_CHOSEN      = 0x0000000000000040,
};

const char *options[] = {
    "--emulate",
    "--debug",
    "--assemble",
    "--config",
    "--version",
    "--help",
    NULL,
};

struct {
    const char *target;
    uint32_t flags;
} cli;

bool str_ends_with(const char *str, const char *suffix) {
    if (!str || !suffix)
        return false;

    size_t len_str = strlen(str);
    size_t len_suf = strlen(suffix);

    if (len_suf > len_str)
        return false;

    return strcmp(str + len_str - len_suf, suffix) == 0;
}

const char *error_strings[] = {
    "Target undefined. Please select a `.asm` or `.bin` file.",
    "No target files."
};

int error(size_t code) {
    printf("ERROR: %s\n", error_strings[code]);
    printf("Use `xm65 --help E%zu` for more information.\n", code);
    return 1;
}

int main(int argc, char *argv[]) {
    cli.target = NULL;
    cli.flags = CLI_NO_FLAGS;

    // Argument parser
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        
        if (arg[0] == '-') {
            size_t option = 0;
            const char* option_str = options[option];

            while (option_str != NULL) {
                int cmp = strcmp(option_str, arg);

                if (cmp == 0) cli.flags |= 1 << option;

                option_str = options[++option];
            }
        } else {
            bool target_asm = str_ends_with(arg, ".asm");
            bool target_bin = str_ends_with(arg, ".bin");

            if (target_asm || target_bin) {
                if (cli.flags & CLI_TARGET_CHOSEN) return error(0);

                cli.flags |= CLI_TARGET_CHOSEN;
                cli.target = arg;
            }
        }
    }

    if (cli.target == NULL) return error(1);

    printf("INFO: Target file: %s\n", cli.target);
    printf("INFO: Flags: %zu\n", cli.flags);

    return 0;
}
