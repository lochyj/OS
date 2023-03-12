#include "include/ports.h"
#include "kernel/inc/util.h"

#include <stdint.h>

void set_cursor(int x, int y) {
	u16 pos = y * VGA_WIDTH + x;

	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (u8) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (u8) ((pos >> 8) & 0xFF));
}

u16 get_cursor() {
    u16 pos = 0;
    port_byte_out(0x3D4, 0x0F);
    pos |= port_byte_in(0x3D5);
    port_byte_out(0x3D4, 0x0E);
    pos |= ((u16)port_byte_in(0x3D5)) << 8;
    return pos;
}

int get_x_from_offset(int offset) {
    return offset % VGA_WIDTH;
}

int get_y_from_offset(int offset) {
    return offset / VGA_WIDTH;
}

void enable_cursor(u8 cursor_start, u8 cursor_end)
{
	port_byte_out(0x3D4, 0x0A);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xC0) | cursor_start);
 
	port_byte_out(0x3D4, 0x0B);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xE0) | cursor_end);
}

void set_char_at_video_memory(char character, u16 offset) {
    unsigned char *videoMemory = (unsigned char *) 0xB8000;
    videoMemory[offset] = character;
    videoMemory[offset + 1] = video_color;
}

u16 get_offset(int x, int y) {
    return (u16)(x * VGA_WIDTH + y);
}

u16 move_offset_to_new_line(int x, int y) {
    return get_offset(0, y + 1);
}

u16 scroll_ln(int x, int y) {
    memory_copy(
        (u8 *) (get_offset(0, 1) + 0xB8000),
        (u8 *) (get_offset(0, 0) + 0xB8000),
        VGA_WIDTH * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < VGA_WIDTH; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return get_offset(x, y) - 2 * VGA_WIDTH;
}

void putc(char chr) {
    u16 offset = get_cursor();

    if (offset >= MAX_ROWS * VGA_WIDTH * 2) {
        offset = scroll_ln(get_x_from_offset(offset), get_y_from_offset(offset));
    }

    if (chr == '\n') {
        offset = move_offset_to_new_line(get_x_from_offset(offset), get_y_from_offset(offset));
    } else if(chr == '\0') {
        set_cursor(get_x_from_offset(offset), get_y_from_offset(offset));
        return;
    } else {
        set_char_at_video_memory(chr, offset);
        offset += 2;
    }

    set_cursor(get_x_from_offset(offset), get_y_from_offset(offset));
}

void print_string(char *string) {
    int i = 0;
    int offset = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * VGA_WIDTH * 2) {
            scroll_ln(get_x_from_offset(offset), get_y_from_offset(offset));
        }
        if (string[i] == '\n') {
            move_offset_to_new_line(get_x_from_offset(offset), get_y_from_offset(offset));
        } else {
            putc(string[i]);
            offset+= 2;
        }
        i++;                                            
    }
    set_cursor(get_x_from_offset(offset), get_y_from_offset(offset));
}

void print_int(int num) {
    char* buffer;
    int_to_string(num, buffer);
    for (int i = 0; i < string_length(buffer); i++) {
        putc(buffer[i]);
    }
}

void print_nl() {
    u16 offset = get_cursor();
    u16 newOffset = move_offset_to_new_line(get_x_from_offset(offset), get_y_from_offset(offset));
    if (newOffset >= MAX_ROWS * MAX_COLS * 2) {
        newOffset = scroll_ln(get_x_from_offset(newOffset), get_y_from_offset(newOffset));
    }
    set_cursor(get_x_from_offset(newOffset), get_y_from_offset(newOffset));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(0, 0);
}

void print_backspace() {
    u16 newCursor = get_cursor() - 2;
    set_char_at_video_memory(' ', newCursor);
    set_cursor(get_x_from_offset(newCursor), get_y_from_offset(newCursor));
}

void print_hex(u8 num) {
    int buffer;
    buffer = hex_to_int(num);
    print_int(buffer);
}
