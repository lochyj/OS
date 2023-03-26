#include "port.h"
#include "types.h"
#include "display.h"

// #if defined(__linux__)
// #error "You are not using a cross-compiler, you will most certainly run into trouble"
// #endif

// #if !defined(__i386__)
// #error "You are using the WRONG compiler, please compile with i386-elf-gcc"
// #endif

void __kernel_preinit() {
    enable_cursor(0x00, 0x0F);
}

void __kernel_main() {
    __kernel_preinit();

    kprint_s("Hello, world!\n");
    kprint_s("\nWOW");
}
