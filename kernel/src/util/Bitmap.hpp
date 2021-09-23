#pragma once

#include <stdint.h>
#include <cstddef>

class Bitmap
{
public:
	Bitmap(uint8_t* _map, size_t _size);
	Bitmap();

	bool operator[](uint64_t idx);
	bool Set(uint64_t idx, bool b);

	size_t size;
	uint8_t* map;
};