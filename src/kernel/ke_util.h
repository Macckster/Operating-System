#pragma once
#include "../cpu/types.h"
#include "ke_string.h"

void memcpy(char* source, char* dest, int nbytes);
void memset(u8* dest, u8 val, u32 len);
void itoa(int n, char str[]);