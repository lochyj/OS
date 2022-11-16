#include "include.h"

#define kernel_version "0.1.0"
#define boot_loader_version "0.2"

void init() {
    clear_screen();
    load_idt();
    isr_install();
    asm volatile("sti");

    add_registry_entry("KEYBOARD_PASSTHROUGH", "true", 0);

    init_keyboard();
}

void printer(char str) {
    print_char(str);
}

void main() {

    init();

    FunctionCallback callback = &printer;

    add_keyboard_sub("printer", callback);
    
}
