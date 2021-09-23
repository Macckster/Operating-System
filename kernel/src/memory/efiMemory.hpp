#pragma once
#include "../types.h"

struct EFI_MEMORY_DESCRIPTOR
{
	uint32_t type;
	void* physAddr;
	void* virtAddr;
	uint64_t pages;
	uint64_t attribs;
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];