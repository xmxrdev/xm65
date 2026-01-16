#pragma once

#include <stdint.h>

char* XM65_ByteToBin(uint8_t);
int XM65_OpenFile(const char*, char**, long*);
void XM65_CloseFile(char*);
