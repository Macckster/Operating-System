#pragma once

#include "core.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "memory.h"

int printf(const char* fmt, ...);
char* read_string();