#pragma once

#include "kernel/cpu/cpu.h"

#include <stdint.h>

#define SC_MAX 57
#define KB_MAX 59


// GLOBALS
static char key_buffer_previous[256];
static char key_buffer[256];

bool waiting_for_input = false;
bool input_returned = false;

bool shift = false;
bool ctrl = false;
bool alt = false;

enum {
    ERROR = 0x00,
    ESCAPE = 0x01,
    BACKSPACE = 0x0E,
    ENTER = 0x1C,
    LEFT_CONTROL = 0x1D,
    LEFT_SHIFT = 0x2A,
    RIGHT_SHIFT = 0x36,
    LEFT_ALT = 0x38,
    SPACE = 0x39,
    CAPS_LOCK = 0x3A,
};


typedef struct keyboard_key {
    u8 scancode;
    bool pressed;
} keyboard_key;

typedef struct keyboard_register {
    keyboard_key keys[KB_MAX];
} keyboard_register;

bool init_keyboard_register(keyboard_register reg);

keyboard_register keyboard_registry;

char *sc_name[] = {
    "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
    "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
    "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
    "/", "RShift", "Keypad *", "LAlt", "Spacebar"
};

char sc_ascii_upper[] = {
    0x00, '~', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '-', '=', 0x00, 0x00, 'Q', 'W', 'E', 'R', 'T', 'Y',
    'U', 'I', 'O', 'P', '{', '}', 0x00, 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ':', '"', '`', 0x00, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0x00, 0x00, 0x00, ' '
};

char sc_ascii_lower[] = {
    0x00, '~', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', 0x00, 0x00, 'q', 'w', 'e', 'r', 't', 'y',
    'u', 'i', 'o', 'p', '[', ']', 0x00, 0x00, 'a', 's', 'd', 'f', 'g',
    'h', 'j', 'k', 'l', ';', '\'', '`', 0x00, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0x00, 0x00, 0x00, ' '
};

void scancode_to_ascii(u8 scancode);

void replace_buffer(char buffer[256], char replacement[256]);

static void keyboard_callback(registers_t *regs);

void init_keyboard();

char* kinput();

#include "kernel/drivers/keyboard.c"