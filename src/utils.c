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
    if (!file) {
        return XM65_ThrowError(2, filename);
    }

    fseek(file, 0, SEEK_END);
    *filesize = ftell(file);
    rewind(file);

    *buffer = (char *) malloc(*filesize);
    if (!*buffer) {
        fclose(file);
        return XM65_ThrowError(3);
    }

    size_t read_bytes = fread(*buffer, 1, *filesize, file);
    if ((long) read_bytes != *filesize) {
        perror("Failed to read file completely");
        free(*buffer);
        fclose(file);
        return 1;
    }

    fclose(file);

    return 0;
}

void XM65_CloseFile(char *buffer) {
    free(buffer);
}
