#include "memory.hpp"
#include "../gfx/print.hpp"

uint64_t GetTotalSystemMemory(EFI_MEMORY_DESCRIPTOR* pMap, uint64_t mapSize, uint64_t mapDescSize)
{
	static uint64_t memorySizeBytes = 0;
	if (memorySizeBytes > 0) return memorySizeBytes;

	for (int i = 0; i < mMapEntries; i++)
	{
	 EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
		memorySizeBytes += desc->numPages * 0x1000;
	}

	return memorySizeBytes;
}

void memset(void* start, uint8_t val, size_t amount)
{
	Printf("Start: %p, val: %d, amount: %d", start, val, amount);

	for (size_t i = 0; i < amount; i++)
	{
		*(uint8_t*)((uint64_t)start + i) = val;
	}
}