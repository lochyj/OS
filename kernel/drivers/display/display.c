#include "kernel/drivers/display.h"

// Info on the pointer and its use. https://wiki.osdev.org/Printing_To_Screen
uint16_t *textmemptr = (uint16_t *)0xB8000;

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

uint8_t interfaceColours = 0x0F;

static void scroll() {
    uint32_t blank, temp;

    // Blank is a space character that has the interfaceColours applied to it.
    blank = 0x20 | (interfaceColours << 8);


    if(cursor_y >= SCREEN_HEIGHT) {
        // Move the current text chunk that makes up the screen back in the buffer by a line
        temp = cursor_y - SCREEN_HEIGHT + 1;
        memcpy(textmemptr, textmemptr + temp * SCREEN_WIDTH, (SCREEN_HEIGHT - temp) * SCREEN_WIDTH * 2);

        // Finally, we clear the last line
        memsetw(textmemptr + (SCREEN_HEIGHT - temp) * SCREEN_WIDTH, blank, SCREEN_WIDTH);
        cursor_y = SCREEN_HEIGHT - 1;
    }
}

void putc(char c) {
    unsigned short *where;
    unsigned short *tempChar;
    unsigned attributes = interfaceColours << 8;

    // Backspace

    switch (c) {
        case 0x08:
            if(cursor_x != 0) cursor_x--;
            tempChar = textmemptr + (cursor_y * SCREEN_WIDTH + cursor_x);
            *tempChar = ' ' | attributes;	/* Character AND attributes: color */
            break;
        
        case 0x09:
            cursor_x = (cursor_x + 8) & ~(8 - 1);
            break;
        
        case '\r':
            cursor_x = 0;
            break;
        
        case '\n':
            cursor_x = 0;
            cursor_y++;
            break;
        
        default:
            break;
    }
    
    // Every character that is and comes after a space is a printable character
    if(c >= ' ') {
        where = textmemptr + (cursor_y * SCREEN_WIDTH + cursor_x);
        *where = c | attributes;	/* Character AND attributes: color */
        cursor_x++;
    }

    // Handle end of screen
    if(cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_csr();
}

void puts(char *text) {
    for (int i = 0; i < strlen(text); i++) {
        putc(text[i]);
    }
}

void initialise_textmode_terminal() {
    enable_cursor(14, 15);
    clear_screen();
}
