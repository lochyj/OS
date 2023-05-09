#include "blink/multiboot.h"
#include <string.h>
#include "kernel/drivers/display.h"

const char* KERNEL_VERSION = "v0.1.2";

void kernel_main(multiboot_info_t* mboot) {

    puts("Hello there!\n");

}