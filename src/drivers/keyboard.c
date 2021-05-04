#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../kernel/ke_util.h"

static void keyboard_callback(registers_t regs)
{
    /* The PIC leaves us the scancode in port 0x60 */
    u8 scancode = port_byte_in(0x60);
    char* sc_ascii;
    itoa(scancode, sc_ascii);
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}