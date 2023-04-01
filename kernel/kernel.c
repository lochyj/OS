#include "system.h"
#include "kernel/system/vmem/page.h"

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

    initialise_paging();

    asm_sti();

    kprintf(" ____  _ _       _     ____   _____\n");
    kprintf("|  _ \\| (_)     | |   / __ \\ / ____|\n");
    kprintf("| |_) | |_ _ __ | | _| |  | | (___\n");
    kprintf("|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n");
    kprintf("| |_) | | | | | |   <| |__| |____) |\n");
    kprintf("|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
    kprintf("Kernel version %s; User: %s\n", KERNEL_VERSION, USER);

    kprintf("terminal@%s> ", USER);

    kprintf("yoo");

    uint32_t *ptr = (uint32_t*)0xA0000000;
    uint32_t do_page_fault = *ptr;

}