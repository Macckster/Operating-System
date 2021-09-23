#include "efiMemory.hpp"

const char* EFI_MEMORY_TYPE_STRINGS[]{
	"EfiResevedMemoryType",
	"EfiLoaderCode",
	"EfiLoaderData",
	"EfiBootServicesCode",
	"EfiBootServicesData",
	"EfiRuntimeServicesCode",
	"EfiRuntimeServicesData",
	"EfiConventionalMemory",
	"EfiUnusableMemory",
	"EfiACPIReclaimMemory",
	"EfiACPIMemoryNVS",
	"EfiMemoryMappedIO",
	"EfiMemoryMappedIOPortSpace",
	"EfiPalCode",
};