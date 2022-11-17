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

    subscribe_to_bus(keyboard_bus, (SubscribeBus) {"keyboard", keyboard_callback});

    // TODO: make this work

    // RegistryEntry entry;

    // RegistryKey key;

    // entry.name = "keyboard";
    // entry.type = 1;
    // key.name = "KEYBOARD";
    // key.key = "Lol F";
    // entry.keys[10] = key;

    // registry_add_entry(entry);

    // for (int i = 0; i < registry_count; i++) {
    //     print_nl();
    //     print_string(registry[i].name);
    //     print_nl();
    //     print_string(registry[i].keys[0].name);
    //     print_nl();
    //     print_string(registry[i].keys[0].key);
    //     print_nl();
    // }

}
