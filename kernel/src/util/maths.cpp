#include "maths.hpp"

uint Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	uint32_t c = 0;

	c |= a << 24;
	c |= r << 16;
	c |= g << 8;
	c |= b;

	return c;
}

uint Colour(uint8_t r, uint8_t g, uint8_t b)
{
	return Colour(r, g, b, 0xff);
}