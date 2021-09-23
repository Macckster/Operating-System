#include "Bitmap.hpp"

Bitmap::Bitmap(uint8_t* _map, size_t _size)
{
	map = _map;
	size = _size;
}

Bitmap::Bitmap() {}

bool Bitmap::operator[](uint64_t idx)
{
	if (idx > size * 8) return false;

	uint64_t byteIdx = idx / 8;
	uint64_t bitIdx = idx % 8;
	uint64_t bitIdxer = 0b10000000 >> bitIdx;

	return (map[byteIdx] & bitIdxer) > 0;
}

bool Bitmap::Set(uint64_t idx, bool b)
{
	if (idx > size * 8) return false;

	uint64_t byteIdx = idx / 8;
	uint64_t bitIdx = idx % 8;
	uint64_t bitIdxer = 0b10000000 >> bitIdx;

	map[byteIdx] &= ~bitIdxer;

	if (b)
	{
		map[byteIdx] |= bitIdxer;
	}

	return true;
}
