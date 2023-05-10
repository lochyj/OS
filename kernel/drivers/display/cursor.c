#include "kernel/drivers/display.h"

int cursor_x = 0, cursor_y = 0;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

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