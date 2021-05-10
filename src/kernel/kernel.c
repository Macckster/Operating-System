#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

#include "string.h"
#include "util.h"

#include "../cpu/isr.h"
#include "../cpu/idt.h"

#include "core.h"
#include "io.h"

void init()
{
    isr_install();
    asm volatile("sti");
    init_keyboard();
    init_memory();
}

void kernel_setup()
{
    ke_println("Entered 32-bit protected mode");

    char strversion[11];
    itoa(KERNEL_VERSION, strversion);

    ke_print("Kernel Version: ");
    ke_println(strversion);

    init();

    ke_println("Kernel is running!");
}

void shell()
{
    while (1)
    {
        ke_print(">");
        char* input = read_string();

        if (!strcmp(input, "whoami"))
        {
            printf("root\n");
        }
        else if (!strcmp(input, "mem"))
        {
            printf("Memory ptr is at: %d\n", get_arr_ptr());
        }


        printf("You said: %s\n", input);

        free(input);
    }
}

void kernel_main()
{
    kernel_setup();
    shell();
}