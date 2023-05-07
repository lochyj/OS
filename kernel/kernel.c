#include "blink/multiboot.h"
#include <string.h>

const char* KERNEL_VERSION = "v0.1.2";

void kernel_main(multiboot_info_t* mboot) {

    volatile char *video = (volatile char*)0xB8000;

    *video = 'A';
    *(video + 1) = 0x07;
    *(video + 2) = 'B';

}