#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "registry/registry.h"
#include "utilities/memory.h"
#include "utilities/time.h"

void init() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling CMOS read for RDT on port 0x70 and 0x71.\n");
    init_time();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing dynamic memory.\n");
    init_dynamic_mem();
    
    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();

    port_byte_out(0x70, 0x0C);	// select register C
    port_byte_in(0x71);		// just throw away contents
    
}

void main() {

    init();

}
