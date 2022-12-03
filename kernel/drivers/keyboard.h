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

// Yes I know this is a bad way to do this, but it works for now
// TODO: make this a function and not have two arrays.
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