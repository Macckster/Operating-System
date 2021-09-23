#pragma once
#include <stdint.h>

#ifdef __cplusplus
#include <cstddef>
#include "memory/efiMemory.hpp"
#else
typedef unsigned long long size_t;
#endif // __cplusplus

typedef struct FrameBuffer
{
	void* baseAddress;
	size_t bufferSize;

	uint32_t width;
	uint32_t height;
	uint32_t pixPerScanline;

} FrameBuffer_t;

#define magic0 0x36
#define magic1 0x04

typedef struct psf1_header
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
}psf1_header_t;

typedef struct psf1_font
{
	psf1_header_t* header;
	void* glyphBuffer;
}psf1_font_t;

typedef struct Boot_info
{
	psf1_font_t* font;
	FrameBuffer_t* buffer;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
}Boot_info_t;