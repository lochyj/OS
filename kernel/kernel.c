#include "system.h"

const char* KERNEL_VERSION = "v0.1.0";
const char* USER = "Lochyj";

void kernel_main() {
    // Initialise the GDT, IDT, ISRs and IRQs
    gdt_install();
    idt_install();
    isr_install();
    irq_install();

    init_video();

    keyboard_install();

    asm_sti();

    kprintf(" ____  _ _       _     ____   _____\n");
    kprintf("|  _ \\| (_)     | |   / __ \\ / ____|\n");
    kprintf("| |_) | |_ _ __ | | _| |  | | (___\n");
    kprintf("|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n");
    kprintf("| |_) | | | | | |   <| |__| |____) |\n");
    kprintf("|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
    kprintf("Kernel version %s; User: %s\n", KERNEL_VERSION, USER);

    kprintf("terminal@%s> ", USER);
}