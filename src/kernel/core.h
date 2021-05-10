#pragma once

#include <stdint.h>
#include <stddef.h>


#define KERNEL_VERSION (COMPILE_TIME / 86400) - 18749

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)