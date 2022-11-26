#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "registry/registry.h"
#include "utilities/memory.h"

void nothing(u8 scancode) {
    // Nothing ¯\_(ツ)_/¯
}

void main() {

    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");
    
    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();

    // Testing the keyboard callbacks ---
    FunctionCallback callback = &nothing;

    SubscribeBus bus = {"keyboard", callback};

    subscribe_to_bus(keyboard_bus, bus, &keyboard_bus_count);

}
