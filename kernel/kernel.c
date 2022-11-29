#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "registry/registry.h"
#include "utilities/memory.h"
#include "utilities/time.h"

void nothing(u8 scancode) {
    // Nothing ¯\_(ツ)_/¯
    print_nl();
    print_string("The time is ");
    print_time(get_kernel_time());
    print_nl();
}

void init() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    port_byte_out(0x70, 0x8B);		// select register B, and disable NMI
    char prev = port_byte_in(0x71);	// read the current value of register B
    port_byte_out(0x70, 0x8B);		// set the index again (a read will reset the index to register D)
    port_byte_out(0x71, prev | 0x40);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing dynamic memory.\n");
    init_dynamic_mem();
    
    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();
}

void main() {

    init();

    // Testing the keyboard callbacks ---
    FunctionCallback callback = &nothing;

    SubscribeBus bus = {"keyboard", callback};

    subscribe_to_bus(keyboard_bus, bus, &keyboard_bus_count);

    print_nl();
    int *ptr1 = alloc(5);
    print_string("int *ptr1 = alloc(5)\n");
    print_dynamic_mem();
    print_nl();

    port_byte_out(0x70, 0x0C);	// select register C
    port_byte_in(0x71);		// just throw away contents
}
