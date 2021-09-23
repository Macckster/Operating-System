#pragma once

#define toKB(x) x / 0x400
#define toMB(x) x / 0x100000
#define toGB(x) x / 0x40000000

#define KERNEL_VERSION (COMPILE_TIME / 86400) - 18749