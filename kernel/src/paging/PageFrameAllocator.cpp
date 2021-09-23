#include "PageFrameAllocator.hpp"
#include "../gfx/print.hpp"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool initialized = false;
PageFrameAllocator gAllocator;

PageFrameAllocator::PageFrameAllocator() {}

void PageFrameAllocator::ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize)
{
    if (initialized) return;

    initialized = true;

    uint64_t mMapEntries = mMapSize / mMapDescSize;

    void* largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;

    for (int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type == 7)
        { // type = EfiConventionalMemory
            if (desc->pages * 0x1000 > largestFreeMemSegSize)
            {
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->pages * 0x1000;
            }
        }
    }

    uint64_t memorySize = GetTotalSystemMemory(mMap, mMapEntries, mMapDescSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 0x1000 / 8 + 1;

    InitBitmap(bitmapSize, largestFreeMemSeg);

    LockPages(&pageBitmap, pageBitmap.size / 0x1000 + 1);

    for (int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type != 7)
        { // not efiConventionalMemory
            ReservePages(desc->physAddr, desc->pages);
        }
    }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    pageBitmap.size = bitmapSize;
    pageBitmap.map = (uint8_t*)bufferAddress;
    for (int i = 0; i < bitmapSize; i++)
    {
        *(uint8_t*)(pageBitmap.map + i) = 0;
    }
}
uint64_t pageBitmapIndex = 0;
void* PageFrameAllocator::RequestPage()
{
    for (; pageBitmapIndex < pageBitmap.size * 8; pageBitmapIndex++)
    {
        point p = g_tRend.GetCursor();
        point a = p;
        a.y = 16 * 16;
        a.x = 0;
        g_tRend.SetCursor(a);
        Printf("PageBitMapIndex is: %d", pageBitmapIndex);
        g_tRend.SetCursor(p);

        if (pageBitmap[pageBitmapIndex] == true) { continue; Printf("Continue!"); while (true) { } };
        LockPage((void*)(pageBitmapIndex * 0x1000));
        return (void*)(pageBitmapIndex * 0x1000);
    }

    return NULL; // Page Frame Swap to file
}

void PageFrameAllocator::FreePage(void* address)
{
    uint64_t index = (uint64_t)address / 0x1000;
    if (pageBitmap[index] == false) return;
    if (pageBitmap.Set(index, false))
    {
        freeMemory += 0x1000;
        usedMemory -= 0x1000;
        if (pageBitmapIndex > index) pageBitmapIndex = index;
    }
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount)
{
    for (int t = 0; t < pageCount; t++)
    {
        FreePage((void*)((uint64_t)address + (t * 0x1000)));
    }
}

void PageFrameAllocator::LockPage(void* address)
{
    uint64_t index = (uint64_t)address / 0x1000;
    if (pageBitmap[index] == true) return;
    if (pageBitmap.Set(index, true))
    {
        freeMemory -= 0x1000;
        usedMemory += 0x1000;
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount)
{
    for (int t = 0; t < pageCount; t++)
    {
        LockPage((void*)((uint64_t)address + (t * 0x1000)));
    }
}

void PageFrameAllocator::ReleasePage(void* address)
{
    uint64_t index = (uint64_t)address / 0x1000;
    if (pageBitmap[index] == false) return;
    if (pageBitmap.Set(index, false))
    {
        freeMemory += 0x1000;
        reservedMemory -= 0x1000;
        if (pageBitmapIndex > index) pageBitmapIndex = index;
    }
}

void PageFrameAllocator::ReleasePages(void* address, uint64_t pageCount)
{
    for (int t = 0; t < pageCount; t++)
    {
        ReleasePage((void*)((uint64_t)address + (t * 0x1000)));
    }
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64_t index = (uint64_t)address / 0x1000;
    if (pageBitmap[index] == true) return;
    if (pageBitmap.Set(index, true))
    {
        freeMemory -= 0x1000;
        reservedMemory += 0x1000;
    }
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount)
{
    for (int t = 0; t < pageCount; t++)
    {
        ReservePage((void*)((uint64_t)address + (t * 0x1000)));
    }
}

uint64_t PageFrameAllocator::GetFreeMemory()
{
	return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedMemory()
{
	return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedMemory()
{
	return reservedMemory;
}
