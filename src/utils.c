#include "utils.h"

char* XM65_ByteToBin(uint8_t byte) {
    static char bits[9];  // 8 bits + null terminator
    for (int i = 0; i < 8; i++) {
        bits[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    bits[8] = '\0';
    return bits;
}
