#include <efi.h>
#include <efilib.h>
#include <elf.h>

#include "../../kernel/src/bootloader_types.h"


FrameBuffer_t framebuffer;

FrameBuffer_t* InitializeGop()
{
	EFI_GUID gopGui = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;

	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGui, NULL, (void**)&gop);

	if (EFI_ERROR(status))
	{
		Print(L"Unable to locate GOP\n\r");
		return NULL;
	}

	Print(L"Gop located\n\r");

	framebuffer.baseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.bufferSize = gop->Mode->FrameBufferSize;

	framebuffer.width = gop->Mode->Info->HorizontalResolution;
	framebuffer.height = gop->Mode->Info->VerticalResolution;
	framebuffer.pixPerScanline = gop->Mode->Info->PixelsPerScanLine;

	return &framebuffer;
}

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if (Directory == NULL)
	{
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS)
	{
		return NULL;
	}
	return LoadedFile;

}

psf1_font_t* LoadPsf1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);

	if (font == NULL)
	{
		return NULL;
	}

	psf1_header_t* fontHeader;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(psf1_header_t), (void**)&fontHeader);
	UINTN size = sizeof(psf1_header_t);

	font->Read(font, &size, fontHeader);

	if (fontHeader->magic[0] != magic0 || fontHeader->magic[1] != magic1)
	{
		return NULL;
	}

	UINTN glyphBufferSize = fontHeader->charsize * (fontHeader->mode == 1 ? 512 : 256);

	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(psf1_header_t));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	psf1_font_t* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(psf1_font_t), (void**)&finishedFont);
	finishedFont->glyphBuffer = glyphBuffer;
	finishedFont->header = fontHeader;

	return finishedFont;
}

int memcmp(const void* aptr, const void* bptr, size_t n)
{
	const unsigned char* a = aptr, * b = bptr;
	for (size_t i = 0; i < n; i++)
	{
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	return 0;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable);

	EFI_FILE* Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);

	if (Kernel == NULL)
	{
		Print(L"Could not load kernel \n\r");
	}
	else
	{
		Print(L"Kernel Loaded Successfully \n\r");
	}

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}

	if (
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT
		)
	{
		Print(L"kernel format is bad\r\n");
	}
	else
	{
		Print(L"kernel header successfully verified\r\n");
	}

	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)
		)
	{
		switch (phdr->p_type)
		{
		case PT_LOAD:
		{
			int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
			Elf64_Addr segment = phdr->p_paddr;
			SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

			Kernel->SetPosition(Kernel, phdr->p_offset);
			UINTN size = phdr->p_filesz;
			Kernel->Read(Kernel, &size, (void*)segment);
			break;
		}
		}
	}

	Print(L"Kernel Loaded\n\r");

	FrameBuffer_t* buffer = InitializeGop();

	psf1_font_t* font = LoadPsf1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable);

	if (font == NULL)
	{
		Print(L"Font is missing or not valid\n\r");
	}
	else
	{
		Print(L"Font found! Size: %d\n\r", font->header->charsize);
	}

	//TODO: Maybe refactor if this breaks when EFI boot services is turned off.
	Boot_info_t data;

	data.buffer = buffer;
	data.font = font;

	EFI_MEMORY_DESCRIPTOR* map = NULL;
	UINTN mapSize, mapKey;
	UINTN descSize;
	UINT32 descVersion;
	{
		SystemTable->BootServices->GetMemoryMap(&mapSize, map, &mapKey, &descSize, &descVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, mapSize, (void**)&map);
		SystemTable->BootServices->GetMemoryMap(&mapSize, map, &mapKey, &descSize, &descVersion);
	}

	data.mMap = map;
	data.mMapSize = mapSize;
	data.mMapDescSize = descSize;

	SystemTable->BootServices->ExitBootServices(ImageHandle, mapKey);

	void (*KernelStart)(Boot_info_t*) = ((__attribute__((sysv_abi)) void(*)(Boot_info_t*)) header.e_entry);
	KernelStart(&data);

	return EFI_SUCCESS; // Exit the UEFI application
}