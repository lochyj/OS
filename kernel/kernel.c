#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "libs/shell/init_shell.h"

void main() {

    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initialising the shell registry.\n");
    init_shell_commands();

    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();
}
