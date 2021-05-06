#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../kernel/ke_util.h"

#define SC_MAX 57

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define SHIFT_DOWN 0x2A
#define SHIFT_UP 0xAA

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
    /* The PIC leaves us the scancode in port 0x60 */
    u8 scancode = port_byte_in(0x60);

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
    }
    else if (scancode == ENTER)
    {
        ke_print_char('\n');
    }
    else
    {
        char c;
        if (shift)
            c = sc_ascii_mod[scancode];
        else
            c = sc_ascii[scancode];

        ke_print_char(c);
    }
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}