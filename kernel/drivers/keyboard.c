#include "kernel/system/isr.h"
#include "port.h"
#include "display.h"
#include "debug.h"
#include "types.h"

int shift = false;
int ctrl = false;
int alt = false;

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = inb(0x60);

    if (scancode > 0x80) {
        return;
    }

    ASSERT(scancode < 0x80);
    
    char letter;

    switch (scancode) {
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
            shift = true;
            break;
        case LEFT_SHIFT + 0x80:
        case RIGHT_SHIFT + 0x80:
            shift = false;
            break;
        case LEFT_CONTROL:
            ctrl = true;
            break;
        case LEFT_CONTROL + 0x80:
            ctrl = false;
            break;
        case LEFT_ALT:
            alt = true;
            break;
        case LEFT_ALT + 0x80:
            alt = false;
            break;
        default:
            break;
    }

    if (shift == true) {
        letter = sc_ascii_upper[(int) scancode];
    } else {
        letter = sc_ascii_lower[(int) scancode];
    }

    char str[2] = {letter, '\0'};

    kprint_s(str);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
    kprintf("init_keyboard called\n");
}