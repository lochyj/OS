#include "include/display.h"
#include "include/keyboard.h"
#include "kernel/utilities/memory.h"
#include "kernel/utilities/time.h"

//#include <stdio.h>

const char* KERNEL_VERSION = "v1.0.0";
const char* USER = "Lochyj";

void kinit() {
    // We clear the screen buffer before we do anything else
    clear_screen();
    print_string("1");

    load_idt();
    print_string("2");
    isr_install();
    print_string("3");


    // Enabling external interrupts
    asm volatile("sti");
    print_string("4");

    // Initializing dynamic memory
    init_dynamic_mem();
}

void login() {
    // Kernel LOGO
    printf( " ____  _ _       _     ____   _____\n"
            "|  _ \\| (_)     | |   / __ \\ / ____|\n"
            "| |_) | |_ _ __ | | _| |  | | (___\n"
            "|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n"
            "| |_) | | | | | |   <| |__| |____) |\n"
            "|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n"
            "Kernel version: %s; User: %s;\n", KERNEL_VERSION, USER);
    printf("terminal@%s> ", USER);
}

   void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = colour;
    }
}

void _kernel_main() {

    //kinit();
    //init_keyboard();
    //login();
    

    clear_screen();
    putc('a');

    //write_string(0x0f, "Hello, There");


}

    // int a = alloc(sizeof(char));
    // a = 'e';
    // printf("%c", a);

    // init_console();