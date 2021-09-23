#include "util/kernelUtil.hpp"

extern "C" void kernel_main(Boot_info* bootInfo)
{
	InitKernel(bootInfo);

	Printf("Im in kernel main\n");

	int* test = (int*)0x80000000000;
	*test = 2;

	while (1) { }

	return;
}