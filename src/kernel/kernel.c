#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

#include "ke_string.h"
#include "ke_util.h"

#include "../cpu/isr.h"
#include "../cpu/idt.h"


void main()
{
    ke_println("Entered 32-bit protected mode");
    ke_println("Kernel is running!");
    ke_println("Hello, World!");

    isr_install();

    //asm volatile("sti");

    init_keyboard();
}