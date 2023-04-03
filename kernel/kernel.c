#include "system.h"
#include "blink/multiboot.h"
#include "debug.h"
#include "kernel/system/vmem/memory.h"
#include "kernel/system/vmem/page.h"

const char* KERNEL_VERSION = "v0.1.2";

void kernel_main(multiboot_info_t* mboot) {

    //ASSERT(mboot->magic & MULTIBOOT_BOOTLOADER_MAGIC);   //!- mboot->magic & 0x2BADB002 should resolve to be 0x11111111 if the magic number is correct

    // Initialise the GDT, IDT, ISRs and IRQs
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    init_video();

    _sti();

    keyboard_install();

    initialise_paging();

    kprintf(" ____  _ _       _     ____   _____\n");
    kprintf("|  _ \\| (_)     | |   / __ \\ / ____|\n");
    kprintf("| |_) | |_ _ __ | | _| |  | | (___\n");
    kprintf("|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n");
    kprintf("| |_) | | | | | |   <| |__| |____) |\n");
    kprintf("|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
    kprintf("Kernel version %s;\n", KERNEL_VERSION);

    kprintf("terminal@%s> ", "test");

    kprintf("ok12");

}