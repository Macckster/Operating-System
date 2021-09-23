#pragma once

#include "../bootloader_types.h"
#include <stdint.h>

uint64_t GetTotalSystemMemory(EFI_MEMORY_DESCRIPTOR* pMap, uint64_t mapSize, uint64_t mapDescSize);

void InitMemory();
void memset(void* start, uint8_t val, size_t amount);
