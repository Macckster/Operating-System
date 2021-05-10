#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../kernel/util.h"

#define SC_MAX 57

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define SHIFT_DOWN 0x2A
#define SHIFT_UP 0xAA

char key_buffer[128];
int buff_tail;

const char sc_ascii[] = 
  { '?', '?', '1', '2', '3', '4', '5', '6','7', '8', '9', '0', '+', 
    '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y','u', 'i', 'o', 'p', 
    '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g','h', 'j', 'k', 'l', 
    ';', '\'', '`', '?', '<', 'z', 'x', 'c', 'v','b', 'n', 'm', 
    ',', '.', '-', '?', '?', '?', ' ' };

const char sc_ascii_mod[] =
 { '?', '?', '!', '"', '#', '¤', '%', '&','/', '(', ')', '=', '?',
   '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y','U', 'I', 'O', 'P',
   '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G','H', 'J', 'K', 'L',
   ';', '\'', '`', '?', '>', 'Z', 'X', 'C', 'V','B', 'N', 'M',
   ',', '.', '_', '?', '?', '?', ' ' };

int shift = 0;

static void keyboard_callback(registers_t regs)
{
    if (!read_input)
        return;

    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = port_byte_in(0x60);

    if (scancode == SHIFT_DOWN)
    {
        shift = 1;
        return;
    }
    if (scancode == SHIFT_UP)
    {
        shift = 0;
    }

    if (scancode > SC_MAX)
        return;

    if (scancode == BACKSPACE)
    {
        backspace();
        key_buffer[buff_tail] = 8;
        buff_tail++;
    }
    else if (scancode == ENTER)
    {
        ke_print_char('\n');
        key_buffer[buff_tail] = '\n';
        buff_tail++;
    }
    else
    {
        char c;
        if (shift)
            c = sc_ascii_mod[scancode];
        else
            c = sc_ascii[scancode];

        ke_print_char(c);
        key_buffer[buff_tail] = c;
        buff_tail++;
    }
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
    buff_tail = 0;
    read_input = 0;
}

char getchar()
{
    while (buff_tail == 0) {}

    buff_tail--;
    return key_buffer[buff_tail];
}

void set_keyboard_read_enable(int state)
{
    read_input = state;
}