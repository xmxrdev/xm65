#pragma once

#include <stdint.h>
#include <stdbool.h>

char* XM65_ByteToBin(uint8_t);
bool XM65_CanReadFile(const char*);
void XM65_OpenFile(const char*, char**, long*);
void XM65_CloseFile(char*);
