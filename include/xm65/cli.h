#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum {
    /* OPTIONS */
    XM65_CLI_NO_FLAGS           = 0x00000000,
    XM65_CLI_EMULATE_TARGET     = 0x00000001,
    XM65_CLI_VM_CONFIG          = 0x00000002,
    XM65_CLI_MEMORY_ZERO        = 0x00000004,
    XM65_CLI_VERSION            = 0x00000008,
    XM65_CLI_HELP               = 0x00000010,

    /* STATES */
    XM65_CLI_TARGET_CHOSEN      = 0x00000020,
};

enum XM65_ERROR_CODE {
    XM65_ERROR_NO_TARGETS,
    XM65_ERROR_TARGET_UNDEFINED,
    XM65_ERROR_FILE_OPEN,
    XM65_ERROR_MEM_ALLOC,
    XM65_ERROR_FILE_READ,
    XM65_ERROR_SDL_INIT,
    XM65_ERROR_SDL_CREATE,
};

enum XM65_INFO_CODE {
    XM65_INFO_TARGET,
    XM65_INFO_FLAGS,
    XM65_INFO_POWERING,
    XM65_INFO_LOADING,
    XM65_INFO_RESETTING,
    XM65_INFO_EMULATION,
    XM65_INFO_SDL_INIT,
};

typedef struct XM65_Cli {
    const char *target;
    uint32_t flags;
} XM65_Cli;

bool str_ends_with(const char*, const char*);
void XM65_ThrowError(enum XM65_ERROR_CODE, ...);
void XM65_PrintInfo(enum XM65_INFO_CODE, ...);
int XM65_ParseArguments(int, char* [], XM65_Cli*);
