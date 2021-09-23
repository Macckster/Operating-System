#pragma once

#include "../bootloader_types.h"
#include "../types.h"
#include "maths.hpp"
#include "../gfx/print.hpp"
#include "../memory/efiMemory.hpp"
#include "../memory/memory.hpp"
#include "../paging/PageFrameAllocator.hpp"
#include "../error/panic.hpp"
#include "util.hpp"
#include "../paging/PageMapIndexer.hpp"
#include "../paging/Paging.hpp"
#include "../paging/PageTableManager.hpp"

void InitKernel(Boot_info* bootInfo);

extern uint64_t _KERNELSTART;
extern uint64_t _KERNELEND;