#include "screen.h"

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }

int get_cursor_pos()
{
	/* Screen cursor position: ask VGA control register (0x3d4) for bytes
	* 14 = high byte of cursor and 15 = low byte of cursor. */

	//Requesting byte 14: high byte of cursor pos
	port_byte_out(0x3d4, 14);
	// Data is returned in VGA data register (0x3d5)
	int position = port_byte_in(0x3d5);
	position = position << 8;

	port_byte_out(0x3d4, 15);
	position += port_byte_in(0x3d5);

	return position;
}

int print_char(int offset, char c, u8 colour)
{
	if (c == '\n')
	{
		int row = get_offset_row(offset);
		offset = get_offset(0, row + 1);
	}
	else
	{
		vga[offset] = c;
		offset++;

		vga[offset] = WHITE_ON_BLACK;
		offset++;
	}

	/* Check if the offset is over screen size and scroll */
	if (offset >= MAX_ROWS * MAX_COLS * 2)
	{
		for (int i = 1; i < MAX_ROWS; i++)
			memcpy(vga + get_offset(0, i),
				   vga + get_offset(0, i - 1),
				   MAX_COLS * 2);

		/* Blank last line */
		char* last_line = vga + get_offset(0, MAX_ROWS - 1);
		for (int i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

		last_line[1] = WHITE_ON_BLACK;

		offset -= 2 * MAX_COLS;
	}

	return offset;
}

void ke_print_char(char c)
{
	int offset = 2 * get_cursor_pos();

	offset = print_char(offset, c, WHITE_ON_BLACK);

	set_cursor_offset(offset);
}

void ke_print(char* pszMsg)
{
	int len = strlen(pszMsg);

	int offset = 2 * get_cursor_pos();

	for (int i = 0; i < len; i++)
	{
		offset = print_char(offset, pszMsg[i], WHITE_ON_BLACK);
	}

	set_cursor_offset(offset);
}

void ke_println(char* pszMsg)
{
	ke_print(pszMsg);
	ke_print("\n");
}

void set_cursor_offset(int offset)
{
	if (offset < 0)
		return;

	/* Similar to get_cursor_offset, but instead of reading we write data */
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void backspace()
{
	int offset = 2 * get_cursor_pos();

	int row = get_offset_row(offset);
	int col = get_offset_col(offset) - 1;

	offset = get_offset(col, row);

	vga[offset] = 0;
	vga[offset + 1] = WHITE_ON_BLACK;

	set_cursor_offset(offset);
}