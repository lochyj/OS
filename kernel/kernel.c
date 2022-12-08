#include "kernel/drivers/vga.h"
#include "kernel/drivers/keyboard.h"
#include "kernel/registry/registry.h"
#include "kernel/utilities/memory.h"
#include "kernel/utilities/time.h"
#include "kernel/utilities/vga_color.h"

#include <stdio.h>

void kinit() {

    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    load_idt();
    isr_install();

    print_string("Enabling external interrupts.\n");
    asm volatile("sti");

    print_string("Initializing dynamic memory.\n");
    init_dynamic_mem();

    print_string_color("This is green!\n", create_vga_color(GREEN_TEXT, BLACK_BACKGROUND));
    print_string_color("This is blue!\n", create_vga_color(BLUE_TEXT, BLACK_BACKGROUND));
    print_string_color("This has a red background!\n", create_vga_color(WHITE_TEXT, RED_BACKGROUND));

}

void kpostinit() {
    /**
     * @brief This function is called after the kernel has finished initializing and is used for testing purposes.
     */

    // ...

    // Init the shell after the kernel has finished initializing and testing has finished
    print_string("Initializing keyboard (IRQ 1).\n> ");
    init_keyboard();
}

void kmain() {

    kinit();

    kpostinit();

}
