#include "memory.hpp"
#include "../gfx/print.hpp"

uint64_t memorySizeBytes;
uint64_t GetTotalSystemMemory(EFI_MEMORY_DESCRIPTOR* pMap, uint64_t mapSize, uint64_t mapDescSize)
{
	if (memorySizeBytes > 0) return memorySizeBytes;

	for (int i = 0; i < mapSize; i++)
	{
	 EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)pMap + (i * mapDescSize));
		memorySizeBytes += desc->pages * 0x1000;
	}

	return memorySizeBytes;
}

void InitMemory()
{
	memorySizeBytes = 0;
}

void memset(void* start, uint8_t val, size_t amount)
{
	for (size_t i = 0; i < amount; i++)
	{
		*(uint8_t*)((uint64_t)start + i) = val;
	}
}