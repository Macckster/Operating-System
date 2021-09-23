#include "kernelUtil.hpp"
#include "../gdt/gdt.hpp"
#include "../interrupts/idt.hpp"
#include "../interrupts/interrupts.hpp"

void InitGDT()
{
	GDTDescriptor descriptor;
	descriptor.size = sizeof(GDT) - 1;
	descriptor.offset = (uint64_t)&DefaultGDT;

	LoadGDT(&descriptor);
}
IDTR idtr;
void InitInterrupts()
{
	idtr.limit = 0x0fff;
	idtr.offset = (uint64_t)gAllocator.RequestPage();
	IDTDescriptorEntry* int_PageFault = (IDTDescriptorEntry*)(idtr.offset + 0xe * sizeof(IDTDescriptorEntry));
	int_PageFault->SetOffset((uint64_t)PageFaultHandler);
	int_PageFault->type_attr = IDT_TA_InterruptGate;
	int_PageFault->selector = 0x08;

	asm("lidt %0" : : "m" (idtr));
}

//TODO: Update this to panic if any of this fails.
void InitKernel(Boot_info* bootInfo)
{
	InitGDT();

	InitPrint(bootInfo->buffer, bootInfo->font);
	g_tRend.Clear(Colour(0,0,0));
	Printf("Printing Works!\n");

	InitMemory();


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

	uint64_t mem = GetTotalSystemMemory(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);
	Printf("System Memory: %ld\n", mem);

	for (uint64_t i = 0; i < mem; i += 0x1000)
	{
		pMan.MapMemory((void*)i, (void*)i);
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

	InitInterrupts();

	g_tRend.SetCursor({0, 0});
	g_tRend.Clear(Colour(0, 0, 0));

	Printf("Kernel Version: %d\n", KERNEL_VERSION);
}