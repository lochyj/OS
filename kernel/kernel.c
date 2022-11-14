#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "libs/registry/registry.h"
// #include "libs/registry/shell_registry.h"

void shell_command() {
    print_string("You just called the shell command! \n Which is stored in the registry!\n");
}

void main() {

    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Creating registry entries.\n");
    add_registry_entry("NAME", "SHELL", 0);
    
    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();
}
