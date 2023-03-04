#include "kernel/cpu/cpu.h"
#include "kernel/inc/util.h"
#include "include/display.h"

#include <stdint.h>

bool init_keyboard_register(keyboard_register reg) {
    u8 keyid = 0x00;
    for (int i = 0; i < KB_MAX; i++) {
        keyboard_key key = {keyid, false};
        reg.keys[i] = key; 
        keyid = keyid + 0x01;
    }

    return 0;
}

void scancode_to_ascii(u8 scancode) {
    char letter = sc_ascii_upper[(int) scancode];
    append(key_buffer, letter);
    char str[2] = {letter, '\0'};
}

void replace_buffer(char buffer[256], char replacement[256]) {
    int length = string_length(buffer);
    for (int i = 0; i < length; i++) {
        print_backspace();
    }
    print_string(replacement);
}

static void keyboard_callback(registers_t *regs) {
    u8 scancode = port_byte_in(0x60);

    // Debug:
    // print_string(" 0x");
    // print_hex(scancode);
    // print_string(" ");

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
    
    if (scancode < SC_MAX) {
        keyboard_registry.keys[scancode].pressed = true;
    } else if (scancode > SC_MAX) {
        keyboard_registry.keys[scancode - 0x80].pressed = false;
    }

    if (scancode > SC_MAX) {
        if ( scancode > SC_MAX + 0x80);
        return;
    };
    
    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } else if (scancode == ENTER) {
        print_nl();
        memory_copy(key_buffer_previous, key_buffer, 256);
        key_buffer[0] = '\0';
        if (waiting_for_input) {
            input_returned = true;
        }
    } else {

        char letter;

        if (shift == true) {
            letter = sc_ascii_upper[(int) scancode];
        } else {
            letter = sc_ascii_lower[(int) scancode];
        }
        
        append(key_buffer, letter);
        char str[2] = {letter, '\0'};
        print_string(str);
    }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

char* kinput() {
    waiting_for_input = true;
    while (!input_returned) {
        {};
    }

    if (!input_returned) return "Error: Input not returned";

    input_returned = false;
    waiting_for_input = false;
    char* out = &key_buffer[0];
    return out;
}