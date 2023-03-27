#include "types.h"
#include "string.h"
#include "debug.h"

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor() {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void update_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void) {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

/*  Source: https://wiki.osdev.org/Printing_To_Screen
    VGA Text Mode Colour Table
    Color number 	Color name
    0 	            Black
    1 	            Blue
    2 	            Green
    3 	            Cyan
    4 	            Red
    5 	            Purple
    6 	            Brown
    7 	            Gray
    8 	            Dark Gray
    9 	            Light Blue
    10 || A 	    Light Green
    11 || B	        Light Cyan
    12 || C	        Light Red
    13 || D	        Light Purple
    14 || E	        Yellow
    15 || F	        White

    VGA Colour Code: 0xYZ
    Where Y is the background color
    And Z is the text color
    EG: 
        0x4F is white text on a red background,
        0x0F is white text on a black background.
*/

void kputc(char c) {
	if(chrcmp(c, '\0') == 0) return;
	
	if(chrcmp(c, '\n') == 0) {
		y++;
		x = 0;
        update_cursor(x, y);
		return;
	}

	if (x >= VGA_WIDTH) {
		x = 0;
		y++;
	}

	ASSERT(x < VGA_WIDTH);

	volatile char* display = (volatile char*) VGA_ADDR;

	display[(y * VGA_WIDTH + x) * 2] = c;
	display[(y * VGA_WIDTH + x) * 2 + 1] = VGA_COLOUR;
	x++;
	update_cursor(x, y);
}

void kprint_s(const char* string) {
    while( *string != 0 ) {
		kputc(*string++);
	}
}
