#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "registry/registry.h"
#include "utilities/memory.h"

void nothing(u8 scancode) {
    // Nothing ¯\_(ツ)_/¯
}

void init() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");
    
    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();

    print_string("Initializing dynamic memory.\n");
    init_dynamic_mem();
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

    RegistryEntry entry;

    entry.name = "test";
    entry.type = 1;

    RegistryKey key;

    key.name = "test";
    key.key = "test";

    entry.keys[1] = key;

    registry_add_entry(entry);

}
