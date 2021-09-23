#pragma once

#include "../memory/efiMemory.hpp"
#include "../memory/memory.hpp"
#include "../util/Bitmap.hpp"

#include <stdint.h>

class PageFrameAllocator
{
public:
	PageFrameAllocator();

	void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* pMap, size_t mapSize, size_t mapDescSize);
	Bitmap pageBitmap;
	uint64_t pageBitmapIndex;
	bool initialized;

	void FreePage(void* address);
	void FreePages(void* address, uint64_t amount);
	void LockPage(void* address);
	void LockPages(void* address, uint64_t amount);

	void* RequestPage();

	uint64_t GetFreeMemory();
	uint64_t GetUsedMemory();
	uint64_t GetReservedMemory();

private:
	void InitBitmap(size_t bitMapSize, void* buffer);
	void ReleasePage(void* address);
	void ReleasePages(void* address, uint64_t amount);
	void ReservePage(void* address);
	void ReservePages(void* address, uint64_t amount);
};

extern PageFrameAllocator gAllocator;