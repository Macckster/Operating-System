#include "PageTableManager.hpp"
#include "PageMapIndexer.hpp"
#include "PageFrameAllocator.hpp"
#include "../memory/memory.hpp"
#include "../gfx/print.hpp"

#include <stdint.h>

PageTableManager::PageTableManager(PageTable* _PML4)
{
	PML4 = _PML4;
}

/// <summary>
/// TODO: Fix this, Borked on real Hardware
/// </summary>
void PageTableManager::MapMemory(void* virtualMemory, void* physicalMemory)
{
    Printf("Entered map memory");

    PageMapIndexer indexer = PageMapIndexer((uint64_t)virtualMemory);
    PageDirectoryEntry PDE;

    Printf(" Made the index");

    PDE = PML4->entries[indexer.PDP_i];
    PageTable* PDP;

    Printf(" Got the entry");

    if (!PDE.GetFlag(PT_Flag::Present))
    {
        Printf(" True case");

        PDP = (PageTable*)gAllocator.RequestPage();

        Printf(" Page Request done");
        Printf(" Memsetting ptr: %p from Line 38", PDP);

        memset(PDP, 0, 0x1000);

        Printf(" Memset done");

        PDE.SetAddress((uint64_t)PDP >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);

        Printf(" Flags set");

        PML4->entries[indexer.PDP_i] = PDE;

        Printf(" Done true");
    }
    else
    {
        Printf(" False case");
        PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        Printf(" Done false");
    }

    Printf(" Created PDP");

    PDE = PDP->entries[indexer.PD_i];
    PageTable* PD;
    if (!PDE.GetFlag(PT_Flag::Present))
    {
        PD = (PageTable*)gAllocator.RequestPage();
        memset(PD, 0, 0x1000);
        PDE.SetAddress((uint64_t)PD >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PDP->entries[indexer.PD_i] = PDE;
    }
    else
    {
        PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
    }

    Printf(" Created PD");

    PDE = PD->entries[indexer.PT_i];
    PageTable* PT;
    if (!PDE.GetFlag(PT_Flag::Present))
    {
        PT = (PageTable*)gAllocator.RequestPage();
        memset(PT, 0, 0x1000);
        PDE.SetAddress((uint64_t)PT >> 12);
        PDE.SetFlag(PT_Flag::Present, true);
        PDE.SetFlag(PT_Flag::ReadWrite, true);
        PD->entries[indexer.PT_i] = PDE;
    }
    else
    {
        PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
    }

    Printf(" Created PT");

    PDE = PT->entries[indexer.P_i];
    PDE.SetAddress((uint64_t)physicalMemory >> 12);
    PDE.SetFlag(PT_Flag::Present, true);
    PDE.SetFlag(PT_Flag::ReadWrite, true);
    PT->entries[indexer.P_i] = PDE;

    Printf(" Done");
}