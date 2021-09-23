#include "Paging.hpp"

void PageDirectoryEntry::SetFlag(PT_Flag f, bool enabled)
{
	uint64_t selector = (uint64_t)1 << f;

	val &= ~selector;

	if (enabled)
		val |= selector;
}

bool PageDirectoryEntry::GetFlag(PT_Flag f)
{
	uint64_t selector = (uint64_t)1 << f;

	return val & selector;
}

void PageDirectoryEntry::SetAddress(uint64_t addr)
{
	addr &= 0x000000ffffffffff;

	val &= 0xfff0000000000fff;
	val |= (addr << 12);
}

uint64_t PageDirectoryEntry::GetAddress()
{
	return (val & 0x000ffffffffff000) >> 12;
}
