#pragma once
#include "Paging.hpp"

class PageTableManager
{
public:
	PageTableManager(PageTable* _PML4);
	PageTable* PML4;

	void MapMemory(void* virtMemory, void* physMemory);

private:

};