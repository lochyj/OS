#include "system.h"

void kernel_main() {
    // Initialise the GDT, IDT, ISRs and IRQs
    gdt_install();
    idt_install();
    isr_install();
    irq_install();

    init_video();

    //timer_install();

    keyboard_install();

    asm_sti();

    puts("Hello, kernel World!");

    // Infinite loop to prevent the kernel from exiting.
    for (;;);
}