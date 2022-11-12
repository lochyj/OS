#include "drivers/vga.h"
#include "drivers/keyboard.h"

void main() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");
    
    print_string("Initializing keyboard (IRQ 1).\n");
    init_keyboard();
}
