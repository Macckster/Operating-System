#pragma once

#include "../types.h"

typedef struct
{
	uint x, y;
}point;

uint Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint Colour(uint8_t r, uint8_t g, uint8_t b);