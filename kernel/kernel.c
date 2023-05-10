#include "blink/multiboot.h"
#include "kernel/drivers/display.h"

#include "kernel/system/gdt.h"

#include <string.h>

const char* KERNEL_VERSION = "v0.1.2";

void kernel_main(multiboot_info_t* mboot) {

    install_gdt();

    initialise_textmode_terminal();

    puts("Hello there!\nWow");

}