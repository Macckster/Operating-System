#include "util/kernelUtil.hpp"

extern "C" void kernel_main(Boot_info* bootInfo)
{
	InitKernel(bootInfo);

	//err::panic("Hello");

	Printf("Im in kernel main\n");

	while (1) { }

	return;
}