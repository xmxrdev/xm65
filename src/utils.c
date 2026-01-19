#include <stdio.h>
#include <stdlib.h>

#include "xm65/utils.h"
#include "xm65/cli.h"

char* XM65_ByteToBin(uint8_t byte) {
    static char bits[9];  // 8 bits + null terminator
    for (int i = 0; i < 8; i++) {
        bits[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    bits[8] = '\0';
    return bits;
}

bool XM65_CanReadFile(const char *filename) {
    if (!filename) return false;

    FILE *file = fopen(filename, "rb");

    if (!file) return false;

    fclose(file);

    return true;
}

void XM65_OpenFile(const char *filename, char **buffer, long *filesize) {
    FILE *file = fopen(filename, "rb");
    if (!file) XM65_ThrowError(XM65_ERROR_FILE_OPEN, filename);

    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    rewind(file);

    *buffer = (char *) malloc((size_t) *filesize);
    if (!*buffer) {
        fclose(file);
        XM65_ThrowError(XM65_ERROR_MEM_ALLOC);
    }

    size_t read_bytes = fread(*buffer, 1, (size_t) *filesize, file);
    if ((long) read_bytes != *filesize) {
        free(*buffer);
        fclose(file);
        XM65_ThrowError(XM65_ERROR_FILE_READ, filename);
    }

    fclose(file);
}

void XM65_CloseFile(char *buffer) {
    if (buffer) free(buffer);
}
