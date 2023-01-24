#include "kernel/drivers/vga.h"
#include "kernel/drivers/keyboard.h"
#include "kernel/utilities/memory.h"
#include "kernel/utilities/time.h"
#include "kernel/utilities/vga_color.h"

#include <stdio.h>

void (*Function) (...);

void kinit() {
    // We clear the screen buffer before we do anything else
    clear_screen();

    load_idt();
    isr_install();

    // Enabling external interrupts
    asm volatile("sti");

    // Initializing dynamic memory
    init_dynamic_mem();
}

/**
 * @brief This function is called after the kernel has finished initializing and is used for testing purposes.
 */
void kpostinit() {
    // Init the shell after the kernel has finished initializing and testing has finished
    print_string("> ");
    init_keyboard();
}

void kmain() {

    kinit();

    kpostinit();

}
