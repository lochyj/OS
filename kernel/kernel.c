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

    // Initializing dynamic memory
    //init_dynamic_mem();
}

void login() {
    // Kernel LOGO
    printf(" ____  _ _       _     ____   _____\n");
    printf("|  _ \\| (_)     | |   / __ \\ / ____|\n");
    printf("| |_) | |_ _ __ | | _| |  | | (___\n");
    printf("|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n");
    printf("| |_) | | | | | |   <| |__| |____) |\n");
    printf("|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
    printf("Kernel version: %s; User: %s;\n%dkb Memory;\n", KERNEL_VERSION, USER);
}

void _kernel_main() {

    kinit();

    init_keyboard();

    login();

    printf("terminal@%s> ", USER);

    // int a = alloc(sizeof(char));
    // a = 'e';
    // printf("%c", a);

    // init_console();

}
