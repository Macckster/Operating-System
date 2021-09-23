#include "kernelUtil.hpp"

//TODO: Update this to panic if any of this fails.
void InitKernel(Boot_info* bootInfo)
{
	InitPrint(bootInfo->buffer, bootInfo->font);

	g_tRend.Clear(Colour(0,0,0));

	Printf("Printing Works!\n");

	gAllocator = PageFrameAllocator();
	gAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	Printf("I have Read the EFI Memory map\n");

	uint64_t kSize = (uint64_t)&_KERNELEND - (uint64_t)&_KERNELSTART;
	uint64_t kPages = kSize / 0x1000 + 1;

	gAllocator.LockPages(&_KERNELSTART, kPages);

	Printf("I locked the kernel pages\n");

	PageTable* PML4 = (PageTable*)gAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	Printf("I Requested PML4 and memset it\n");

	PageTableManager pMan = PageTableManager(PML4);

	Printf("I made a page table manager\n");

	Printf("pMap: %p, mapSize: %d, mapDescSize: %d", bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	//Den här funktionen är fucket om man använder den här (ie man kan inte calla den) men funkar på andra ställen. Memset behöver delayas eller nåt med ett printf.
	uint64_t mem = 8589934592; //GetTotalSystemMemory(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);
	Printf("I found memory\n");
	Printf("mem: %ld\n", mem);

	//Den här loopen går fan inte å lita på. Jag vette fan vad problemet är.
	for (uint64_t i = 0; i < mem; i += 0x1000)
	{
		pMan.MapMemory((void*)i, (void*)i);
		Printf("Loop: %d/%d", i, mem);
		point p = g_tRend.GetCursor();
		p.x = 0;
		g_tRend.SetCursor(p);
	}

	Printf("I mapped the memory\n");

	uint64_t fbBase = (uint64_t)bootInfo->buffer->baseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->buffer->bufferSize + 0x1000;

	gAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);

	Printf("Mapping frame buffer");

	for (uint64_t i = fbBase; i < fbBase + fbSize; i += 0x1000)
	{
		pMan.MapMemory((void*)i, (void*)i);
		point p = g_tRend.GetCursor();
		p.x = 0;
		g_tRend.SetCursor(p);
	}

	Printf("I mapped the framebuffer\n");

	asm("mov %0, %%cr3" : : "r" (PML4));

	g_tRend.SetCursor({0, 0});

	g_tRend.Clear(Colour(0, 0, 0));

	Printf("Kernel Version: %d\n", KERNEL_VERSION);
}