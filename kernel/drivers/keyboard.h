#include "kernel/shell.h"
#include "kernel/cpu/cpu.h"
#include "kernel/inc/util.h"
#include "kernel/inc/types.h"
#include "kernel/drivers/vga.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

static char key_buffer_previous[256];
static char key_buffer[256];

#define SC_MAX 57
#define KB_MAX 59

enum {
    ERROR = 0x00,
    ESCAPE = 0x01,
    ONE = 0x02,
    TWO = 0x03,
    THREE = 0x04,
    FOUR = 0x05,
    FIVE = 0x06,
    SIX = 0x07,
    SEVEN = 0x08,
    EIGHT = 0x09,
    NINE = 0x0A,
    ZERO = 0x0B,
    MINUS = 0x0C,
    EQUALS = 0x0D,
    BACKSPACE = 0x0E,
    TAB = 0x0F,
    Q = 0x10,
    W = 0x11,
    E = 0x12,
    R = 0x13,
    T = 0x14,
    Y = 0x15,
    U = 0x16,
    I = 0x17,
    O = 0x18,
    P = 0x19,
    LEFT_BRACKET = 0x1A,
    RIGHT_BRACKET = 0x1B,
    ENTER = 0x1C,
    LEFT_CONTROL = 0x1D,
    A = 0x1E,
    S = 0x1F,
    D = 0x20,
    F = 0x21,
    G = 0x22,
    H = 0x23,
    J = 0x24,
    K = 0x25,
    L = 0x26,
    SEMICOLON = 0x27,
    SINGLE_QUOTE = 0x28,
    GRAVE = 0x29,
    LEFT_SHIFT = 0x2A,
    BACKSLASH = 0x2B,
    Z = 0x2C,
    X = 0x2D,
    C = 0x2E,
    V = 0x2F,
    B = 0x30,
    N = 0x31,
    M = 0x32,
    COMMA = 0x33,
    PERIOD = 0x34,
    FORWARD_SLASH = 0x35,
    RIGHT_SHIFT = 0x36,
    NUMPAD_ASTERISK = 0x37,
    LEFT_ALT = 0x38,
    SPACE = 0x39,
    CAPS_LOCK = 0x3A,
    F1 = 0x3B,
    F2 = 0x3C,
    F3 = 0x3D,
    F4 = 0x3E,
    F5 = 0x3F,
    F6 = 0x40,
    F7 = 0x41,
    F8 = 0x42,
    F9 = 0x43,
    F10 = 0x44,
    NUM_LOCK = 0x45,
    SCROLL_LOCK = 0x46,
    NUMPAD_7 = 0x47,
    NUMPAD_8 = 0x48,
    NUMPAD_9 = 0x49,
    NUMPAD_MINUS = 0x4A,
    NUMPAD_4 = 0x4B,
    NUMPAD_5 = 0x4C,
    NUMPAD_6 = 0x4D,
    NUMPAD_PLUS = 0x4E,
    NUMPAD_1 = 0x4F,
    NUMPAD_2 = 0x50,
    NUMPAD_3 = 0x51,
    NUMPAD_0 = 0x52,
    NUMPAD_PERIOD = 0x53,
    F11 = 0x57,
    F12 = 0x58,

    ARROWS = 0x244,
    LEFT_ARROW = 0x4B,
    RIGHT_ARROW = 0x4D,
    UP_ARROW = 0x48,
    DOWN_ARROW = 0x50
};


typedef struct keyboard_key {
    u8 scancode;
    bool pressed;
} keyboard_key;

typedef struct keyboard_register {
    keyboard_key keys[KB_MAX];
} keyboard_register;

bool init_keyboard_register(keyboard_register reg) {

    for (int i = 0; i <= KB_MAX; i++) {
        keyboard_key key = {i * 0x01, false};
        reg.keys[i] = key;
    }

    return 0;
}

keyboard_register keyboard_registry;

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

u8 prev_code;

static void keyboard_callback(registers_t *regs) {
    u8 scancode = port_byte_in(0x60);

    print_string(" 0x");
    print_hex(scancode);
    print_string(" ");

    //* Function temp values
    bool c_shift = false;
    bool c_ctrl = false;

    bool arrow = false;

    // If the last character returned was a shift or ctrl, set the temp values to true and then revert the shift and ctrl values to false
    if (shift == true) {
        c_shift = true;
    } else if (ctrl == true) {
        c_ctrl = true;
    }

    shift = false;
    ctrl = false;

    if (scancode == 0x224) {
        arrow = true;
    }

    if (prev_code == 0x224) {
        if (scancode == UP_ARROW) {
            print_string("UP_ARROW");
            replace_buffer(key_buffer, key_buffer_previous);
        }
    }

    if (scancode < SC_MAX) {
        keyboard_registry.keys[scancode].pressed = true;
    } else if (scancode > SC_MAX) {
        keyboard_registry.keys[scancode - 0x80].pressed = false;
    }

    if (scancode > SC_MAX) {
        if ( scancode > SC_MAX + 0x80);
        // scancode -= 0x80;
        // if (scancode > SC_MAX) {
        //     return;
        // }
        return;
    };
    
    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } else if (scancode ==ENTER) {
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

    prev_code = scancode;
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

#endif