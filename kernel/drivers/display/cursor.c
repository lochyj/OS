#include "kernel/drivers/display.h"

// Info on the pointer and its use. https://wiki.osdev.org/Printing_To_Screen
static uint16_t *textmemptr = (uint16_t *)0xB8000;
static int text_color = 0x0F;

static const int SCREEN_WIDTH = 80;
static const int SCREEN_HEIGHT = 25;

static int cursor_x = 0, cursor_y = 0;

static uint8_t interfaceColours = 0x0F;

void move_csr() {
    unsigned int offset;

    // cursor_y * SCREEN_WIDTH is the offset to the row, cursor_x is the offset to the column
    offset = cursor_y * SCREEN_WIDTH + cursor_x;

    outb(0x3D4, 14);
    outb(0x3D5, offset >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, offset);
}

void clear_screen() {
    unsigned blank;
    int i;

    blank = 0x20 | (interfaceColours << 8);

    for(i = 0; i < 25; i++)
        memsetw(textmemptr + i * SCREEN_WIDTH, blank, SCREEN_WIDTH);

    cursor_x = 0;
    cursor_y = 0;
    move_csr();
}