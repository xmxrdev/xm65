#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "cli.h"

char* XM65_ByteToBin(uint8_t byte) {
    static char bits[9];  // 8 bits + null terminator
    for (int i = 0; i < 8; i++) {
        bits[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    bits[8] = '\0';
    return bits;
}

int XM65_OpenFile(const char *filename, char **buffer, long *filesize) {
    FILE *file = fopen(filename, "rb");
    if (!file) XM65_ThrowError(XM65_ERROR_FILE_OPEN, filename);

    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    rewind(file);

    *buffer = (char *) malloc(*filesize);
    if (!*buffer) {
        fclose(file);
        XM65_ThrowError(XM65_ERROR_MEM_ALLOC);
    }

    size_t read_bytes = fread(*buffer, 1, *filesize, file);
    if ((long) read_bytes != *filesize) {
        free(*buffer);
        fclose(file);
        XM65_ThrowError(XM65_ERROR_FILE_READ, filename);
    }

    fclose(file);

    return 0;
}

void XM65_CloseFile(char *buffer) {
    if (buffer) free(buffer);
}
