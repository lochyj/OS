#include "port.h"
#include "types.h"
#include "display.h"
#include "multiboot.h"
#include "debug.h"

#include "kernel/system/gdt/gdt.h"

#include "kernel/system/interrupt/idt.h"
#include "kernel/system/interrupt/irq.h"
#include "kernel/system/interrupt/isr.h"

#include "kernel/drivers/keyboard.h"

#include "Blink/kprintf.h"

const char* KERNEL_VERSION = "v0.1.0";
const char* USER = "Lochyj";

void __kernel_preinit(struct multiboot_info* mbt) {

    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbt->flags >> 6 & 0x1)) {
        PANIC("invalid memory map given by GRUB bootloader");
    }

    // Enable the interrupt service routines and the interrupt descriptor table
    gdt_install();
	idt_install();
	isr_install();
	irq_install();

    // We need manually enable the vga cursor because GRUB sometimes disables it
    // It is good practice to enable the cursor and it allows us to customize the height (pixel) of the cursor.
    enable_cursor(0x0D, 0x0F);

    // Enable the keyboard ISR to allow for keyboard input
    init_keyboard();

    // We return nothing...
}

void __kernel_main(struct multiboot_info* mbt) {
    __kernel_preinit(mbt);

    kprintf(" ____  _ _       _     ____   _____\n");
    kprintf("|  _ \\| (_)     | |   / __ \\ / ____|\n");
    kprintf("| |_) | |_ _ __ | | _| |  | | (___\n");
    kprintf("|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n");
    kprintf("| |_) | | | | | |   <| |__| |____) |\n");
    kprintf("|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
    kprintf("Kernel version %s; User: %s\n", KERNEL_VERSION, USER);

    kprintf("terminal@%s> ", USER);

    kprintf("\n IT WORKS!");
    
}
