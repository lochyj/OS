#include "include.h"

#define kernel_version "0.1.0"
#define boot_loader_version "0.2"

void init() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();
}

void main() {

    init();

    
    
}
