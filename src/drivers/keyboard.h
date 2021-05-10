#pragma once

#include "../kernel/core.h"

static int read_input;

void init_keyboard();
char getchar();

void set_keyboard_read_enable(int state);