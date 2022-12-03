// TODO: make not dependant on ../

#include "kernel/inc/types.h"
#include "kernel/registry/registry.h"
#include "kernel/cpu/cpu.h"
#include "kernel/shell.h"
#include "kernel/drivers/vga.h"
#include "kernel/inc/util.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer_previous[256];
static char key_buffer[256];

#define SC_MAX 57

#define UP_ARROW 0x48       // Source:
#define DOWN_ARROW 0x50     // https://stackoverflow.com/questions/23188540/what-are-the-scan-codes-for-keyboard-arrows-right-left-down-up
#define LEFT_ARROW 0x4B
#define RIGHT_ARROW 0x4D

typedef struct keyboard_key {
    u8 scancode;
    bool pressed;
} keyboard_key;
typedef struct keyboard_register {
    keyboard_key error; keyboard_key esc; keyboard_key one; keyboard_key two; keyboard_key three; 
    keyboard_key four; keyboard_key five; keyboard_key six; keyboard_key seven; keyboard_key eight;
    keyboard_key nine; keyboard_key zero; keyboard_key dash; keyboard_key equals; keyboard_key backspace;
    keyboard_key tab; keyboard_key q; keyboard_key w; keyboard_key e; keyboard_key r; 
    keyboard_key t; keyboard_key y; keyboard_key u; keyboard_key i; keyboard_key o;
    keyboard_key p; keyboard_key open_bracket; keyboard_key close_bracket; keyboard_key enter; keyboard_key ctrl;
    keyboard_key a; keyboard_key s; keyboard_key d; keyboard_key f; keyboard_key g;
    keyboard_key h; keyboard_key j; keyboard_key k; keyboard_key l; keyboard_key semicolon;
    keyboard_key single_quote; keyboard_key backtick; keyboard_key left_shift; keyboard_key backslash; keyboard_key z;
    keyboard_key x; keyboard_key c; keyboard_key v; keyboard_key b; keyboard_key n;
    keyboard_key m; keyboard_key comma; keyboard_key period; keyboard_key forward_slash; keyboard_key right_shift;
    keyboard_key print_screen; keyboard_key alt; keyboard_key space;
} keyboard_register;

keyboard_register init_keyboard_register() {
    keyboard_register reg = {
        {0x00, false},{0x01, false},{0x02, false},{0x03, false},{0x04, false},{0x05, false},{0x06, false},{0x07, false},
        {0x08, false},{0x09, false},{0x0A, false},{0x0B, false},{0x0C, false},{0x0D, false},{0x19, false},{0x1A, false},
        {0x1B, false},{0x1C, false},{0x1D, false},{0x1E, false},{0x1F, false},{0x20, false},{0x21, false},{0x22, false},
        {0x23, false},{0x24, false},{0x25, false},{0x26, false},{0x27, false},{0x28, false},{0x29, false},{0x2A, false},
        {0x2B, false},{0x2C, false},{0x2D, false},{0x2E, false},{0x2F, false},{0x30, false},{0x31, false},{0x32, false},
        {0x33, false},{0x34, false},{0x35, false},{0x36, false},{0x37, false},{0x38, false},{0x39, false}
    };
    return reg;
}

// TODO: This is not working at the moment...

// char vga_font_array[] {
//     '☺', '☻', '♥', '♦', '♣',
//      '♠', '•', '◘', '○', '◙',
//       '♂', '♀', '♪', '♫', '☼',
//        '►', '◄', '↕', '‼', '¶',
//         '§', '▬', '↨', '↑', '↓',
//          '→', '←', '∟', '↔', '▲',
//           '▼', '!', '"', '#', '$',
//            '%', '&', '\'', '(', ')',
//             '*', '+', ',', '-', '.',
//              '/', '0', '1', '2', '3',
//               '4', '5', '6', '7', '8',
//                '9', ':', ';', '<', '=',
//                 '>', '?', '@', 'A', 'B',
//                  'C', 'D', 'E', 'F', 'G',
//                   'H', 'I', 'J', 'K', 'L',
//                    'M', 'N', 'O', 'P', 'Q',
//                     'R', 'S', 'T', 'U', 'V',
//                      'W', 'X', 'Y', 'Z', '[',
//                       '\\', ']', '^', '_', '`',
//                        'a', 'b', 'c', 'd', 'e',
//                         'f', 'g', 'h', 'i', 'j',
//                          'k', 'l', 'm', 'n', 'o',
//                           'p', 'q', 'r', 's', 't',
//                            'u', 'v', 'w', 'x', 'y',
//                             'z', '{', '|', '}', '~',
//                              '⌂', 'Ç', 'ü', 'é', 'â',
//                               'ä', 'à', 'å', 'ç', 'ê',
//                                'ë', 'è', 'ï', 'î', 'ì',
//                                 'Ä', 'Å', 'É', 'æ', 'Æ',
//                                  'ô', 'ö', 'ò', 'û', 'ù',
//                                   'ÿ', 'Ö', 'Ü', '¢', '£',
//                                    '¥', '₧', 'ƒ', 'á', 'í',
//                                     'ó', 'ú', 'ñ', 'Ñ', 'ª',
//                                      'º', '¿', '⌐', '¬', '½',
//                                       '¼', '¡', '«', '»', '░',
//                                        '▒', '▓', '│', '┤', '╡',
//                                         '╢', '╖', '╕', '╣', '║',
//                                          '╗', '╝', '╜', '╛', '┐',
//                                           '└', '┴', '┬', '├', '─',
//                                            '┼', '╞', '╟', '╚', '╔',
//                                             '╩', '╦', '╠', '═', '╬',
//                                              '╧', '╨', '╤', '╥', '╙',
//                                               '╘', '╒', '╓', '╫', '╪',
//                                                '┘', '┌', '█', '▄', '▌',
//                                                 '▐', '▀', 'ɑ', 'ϐ', 'ᴦ',
//                                                  'ᴨ', '∑', 'ơ', 'µ', 'ᴛ',
//                                                   'ɸ', 'ϴ', 'Ω', 'ẟ', '∞',
//                                                    '∅', '∈', '∩', '≡', '±',
//                                                     '≥', '≤', '⌠', '⌡', '÷',
//                                                      '≈', '°', '∙', '·', '√',
//                                                       'ⁿ', '²', '■',' '
// };

char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

char sc_ascii_upper[] = {
                         '?', '~', '!', '@', '#', '$', '%', '^',
                         '&', '*', '(', ')', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ':', '"', '`', '?', '|', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '
                         };

char sc_ascii_lower[] = {
                         '?', '~', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
                         'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
                         'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                         'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '
                         };

void scancode_to_ascii(u8 scancode) {
    char letter = sc_ascii_upper[(int) scancode];
    append(key_buffer, letter);
    char str[2] = {letter, '\0'};
}

bool shift = false;
bool ctrl = false;

void replace_buffer(char buffer[256], char replacement[256]) {
    int length = string_length(buffer);
    for (int i = 0; i < length; i++) {
        print_backspace();
    }
    print_string(replacement);
}

static void keyboard_callback(registers_t *regs) {
    u8 scancode = port_byte_in(0x60);

    

    // print_string(" 0x");
    // print_hex(scancode);

    //* Function temp values
    bool c_shift = false;
    bool c_ctrl = false;

    // If the last character returned was a shift or ctrl, set the temp values to true and then revert the shift and ctrl values to false
    if (shift == true) {
        c_shift = true;
    } else if (ctrl == true) {
        c_ctrl = true;
    }

    shift = false;
    ctrl = false;

    if (scancode > SC_MAX) {
        // scancode -= 0x80;
        // if (scancode > SC_MAX) {
        //     return;
        // }
        return;
    };

    if (scancode == UP_ARROW) {
        print_string("UP_ARROW");
        replace_buffer(key_buffer, key_buffer_previous);
    } else if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } else if (scancode == ENTER) {
        print_nl();
        execute_shell_input(key_buffer);
        memory_copy(key_buffer_previous, key_buffer, 256);
        key_buffer[0] = '\0';
    } else {
        if (compare_string(sc_name[(int) scancode], "LShift") == 0 || compare_string(sc_name[(int) scancode], "RShift")  == 0) {
            shift = true;
            return;
        } else if (compare_string(sc_name[(int) scancode], "Lctrl") == 0) {
            ctrl = true;
            return;
        }

        char letter;

        if (c_shift == true) {
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

#endif