#pragma once

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

enum {
    ERR = 0x00,
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

static void keyboard_callback(registers_t *regs);
void init_keyboard();

#include "kernel/drivers/keyboard.c"
