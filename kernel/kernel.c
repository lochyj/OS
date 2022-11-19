#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "registry/registry.h"

void main() {

    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");
    
    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();

    FunctionCallback callback = &nothing;

    SubscribeBus bus = {"keyboard", callback};

    subscribe_to_bus(&keyboard_bus, bus, &keyboard_bus_count);

    for (int i = 0; i < keyboard_bus_count; i++) {
        print_string(keyboard_bus[i].name);
        print_nl();
    }

}
