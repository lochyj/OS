#include "kernel/drivers/vga.h"
#include "kernel/drivers/keyboard.h"
#include "kernel/utilities/memory.h"
#include "kernel/utilities/time.h"
#include "kernel/console.h"

#include <stdio.h>

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

void kmain() {

    kinit();

    printf("> ");

    init_keyboard();

}
