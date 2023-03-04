#include "include/display.h"
#include "include/keyboard.h"
#include "kernel/utilities/memory.h"
#include "kernel/utilities/time.h"

#include <stdio.h>

const char* KERNEL_VERSION = "v1.0.0";
const char* USER = "Lochyj";

void kinit() {
    // We clear the screen buffer before we do anything else
    clear_screen();

    load_idt();
    isr_install();

    // Enabling external interrupts
    asm volatile("sti");

    // Dynamic MEM is currently not working...
    // Initializing dynamic memory
    //init_dynamic_mem();
}

void login() {
    // Kernel LOGO
    printf(" ____  _ _       _     ____   _____\n|  _ \\| (_)     | |   / __ \\ / ____|\n| |_) | |_ _ __ | | _| |  | | (___\n|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n| |_) | | | | | |   <| |__| |____) |\n|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
    printf("Kernel version %s; User: %s\n", KERNEL_VERSION, USER);
}

void kmain() {

    kinit();

    login();

    init_keyboard();

    printf("terminal@%s> ", USER);

}
