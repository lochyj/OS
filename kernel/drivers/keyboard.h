#include "../cpu/cpu.h"
#include "../shell.h"
#include "../include.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];

#define SC_MAX 57

const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

subscription_bus keyboard_bus[1001];
int keyboard_bus_index = 0;

void add_keyboard_sub(char* name, FunctionCallback callback) {
    keyboard_bus[keyboard_bus_index].name = name;
    keyboard_bus[keyboard_bus_index].callback = callback;
    keyboard_bus_index++;
}

void remove_keyboard_sub(char* name) {
    for (int i = 0; i < keyboard_bus_index; i++) {
        if (compare_string(keyboard_bus[i].name, name) == 0) {
            keyboard_bus[i].name = "";
            keyboard_bus[i].callback = 0;
        }
    }
}

static void keyboard_callback(registers_t *regs) {
    u8 scancode = port_byte_in(0x60);
    if (scancode > SC_MAX) return;
    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } else if (scancode == ENTER && get_registry_entry_by_type(0)[0].value == "true") {
        print_nl();
        execute_command(key_buffer);
        key_buffer[0] = '\0';
    } else {
        char letter = sc_ascii[(int) scancode];
        append(key_buffer, letter);
        char str[2] = {letter, '\0'};
        print_string(str);
    }

    for (int i = 0; i < keyboard_bus_index; i++) {
        if (keyboard_bus[i].callback != 0) {
            keyboard_bus[i].callback(scancode);
        }
    }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

#endif