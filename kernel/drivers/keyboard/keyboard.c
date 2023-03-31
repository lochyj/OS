#include "system.h"
#include "debug.h"

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

int shift = false;
int ctrl = false;
int alt = false;

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r) {
    uint8_t scancode = inb(0x60);

	if (scancode == LEFT_SHIFT + 0x80 || scancode == RIGHT_SHIFT + 0x80 || scancode == LEFT_CONTROL + 0x80 || scancode == LEFT_ALT + 0x80) {
		// nothing to see here...
	} else if(scancode > 0x80) {
		return;
	}

    //ASSERT(scancode < 0x80);
    
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
		case BACKSPACE:
			putc(0x08);
		case ENTER:
			putc('\n');
        default:
            break;
    }

    if (shift == true) {
        letter = sc_ascii_upper[(int) scancode];
    } else {
        letter = sc_ascii_lower[(int) scancode];
    }

	char str[2] = {letter, '\0'};

    puts(str);
}

void keyboard_install() {
    irq_install_handler(1, keyboard_handler);
}

