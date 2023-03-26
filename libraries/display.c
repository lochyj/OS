#include "types.h"
#include "string.h"

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

uint16_t get_offset(uint16_t x, uint16_t y) {
    return (x * VGA_WIDTH + y);
}

uint16_t set_new_line(uint16_t pos) {
	return get_offset(0, (pos / VGA_WIDTH) + 1);
}

uint16_t VGA_LOC = 0;

void kputc(char c) {
	if(chrcmp(c, '\0') == 0) return;

	volatile char *vga = (volatile char *)VGA_ADDR;
	uint16_t location = VGA_LOC;

	if( chrcmp(c, '\n') == 0) {
		location = set_new_line(location);
		VGA_LOC = location;
		update_cursor(location / VGA_WIDTH, location % VGA_WIDTH);
		return;
	}
	vga[location] = c;
	vga[location + (uint16_t)1] = VGA_COLOUR;

	// We incease the location by 2 because the vga Buffer takes 2 bytes as input for each character displayed: 0xCCcc cc is where the 8bit colour code is stored and CC is the character code.
	location += (uint16_t)2;

	VGA_LOC = location;
	update_cursor(location / VGA_WIDTH, location % VGA_WIDTH);
}

void kprint_s(char* string) {
    for (int i = 0; string[i] != 0; i++) {
		kputc(string[i]);
    }
}
