#pragma once
#include "../kernel/ke_string.h"
#include "ports.h"

#define VGA_ADDRESS 0xb8000
static char* const vga = (char*)VGA_ADDRESS;
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */ 
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

int get_cursor_pos();
void ke_print(char* pszMsg);
void ke_print_char(char c);
void ke_println(char* pszMsg);
void set_cursor_offset(int offset);

void backspace();