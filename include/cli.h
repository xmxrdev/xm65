#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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

static struct {
    const char *target;
    uint32_t flags;
} cli;

static const char *options[] = {
    "--emulate",
    "--debug",
    "--assemble",
    "--config",
    "--version",
    "--help",
    NULL,
};

static const char *error_strings[] = {
    "Target undefined. Please select a `.asm` or `.bin` file.",
    "No target files.",
};

bool str_ends_with(const char *str, const char *suffix);
int error(size_t code);
int parse_arguments(int argc, char *argv[]);
