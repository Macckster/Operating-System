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

void set_cursor_offset(int offset)
{
	/* Similar to get_cursor_offset, but instead of reading we write data */
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int print_char(char c, int col, int row, char attr)
{
	unsigned char* vidmem = (unsigned char*)VGA_ADDRESS;

	if (!attr) 
		attr = WHITE_ON_BLACK;

	/* Error control: print a red 'E' if the coords aren't right */
	if (col >= MAX_COLS || row >= MAX_ROWS)
	{
		vidmem[2 * (MAX_COLS) * (MAX_ROWS)-2] = 'E';
		vidmem[2 * (MAX_COLS) * (MAX_ROWS)-1] = RED_ON_WHITE;

		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0) 
		offset = get_offset(col, row);
	else 
		offset = VGA_OFFSET;

	if (c == '\n')
	{
		row = get_offset_row(offset);
		offset = get_offset(0, row + 1);
	}
	else
	{
		vidmem[offset] = c;
		vidmem[offset + 1] = attr;
		offset += 2;
	}

	set_cursor_offset(offset);
	return offset;
}

void ke_print(char* pszMsg)
{
	get_cursor_pos();

	int local_offset = VGA_OFFSET;

	char* screen = VGA_ADDRESS;

	for (int i = 0; i < ke_strlen(pszMsg); i++)
	{
		screen[local_offset] = pszMsg[i];
		local_offset++;

		screen[local_offset] = WHITE_ON_BLACK;
		local_offset++;
	}

	set_cursor_offset(local_offset);
}

void ke_println(char* pszMsg)
{
	ke_print(pszMsg);

	int newRow = get_offset_row(VGA_OFFSET) + 1;

	int offset = get_offset(0, newRow);

	set_cursor_offset(offset);	
}