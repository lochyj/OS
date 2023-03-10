#include "include/ports.h"
#include "kernel/inc/util.h"

#include <stdint.h>

void set_cursor(int offset) {
    offset;
	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (u8) (offset & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (u8) ((offset >> 8) & 0xFF));
}

int get_cursor() {
    u16 pos = 0;
    port_byte_out(0x3D4, 0x0F);
    pos |= port_byte_in(0x3D5);
    port_byte_out(0x3D4, 0x0E);
    pos |= ((u16)port_byte_in(0x3D5)) << 8;
    return pos;
}

// void enable_cursor(u8 cursor_start, u8 cursor_end)
// {
// 	port_byte_out(0x3D4, 0x0A);
// 	port_byte_out(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
// 	port_byte_out(0x3D4, 0x0B);
// 	port_byte_out(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
// }

void set_char_at_video_memory(char character, int offset) {
    unsigned char *videoMemory = (unsigned char *) VIDEO_ADDRESS;
    videoMemory[offset] = character;
    videoMemory[offset + 1] = video_color;
}

int get_row_from_offset(int offset) {
    return (offset * 2) / (2 * MAX_COLS);
}

int get_offset(int col, int row) {
    return (row * MAX_COLS + col);
}

int move_offset_to_new_line(int offset) {
    return get_offset(0, get_row_from_offset(offset) + 1);
}

int scroll_ln(int offset) {
    memory_copy(
            (u8 *) (get_offset(0, 1) + VIDEO_ADDRESS),
            (u8 *) (get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}

void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_at_video_memory(string[i], offset);
            offset += 2;
        }
        i++;                                            
    }
    set_cursor(offset);
}

void putc(char chr) {
    int offset = get_cursor();

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        offset = scroll_ln(offset);
    }

    if (chr == '\n') {
        offset = move_offset_to_new_line(offset);
    } else {
        set_char_at_video_memory(chr, offset);
        offset += 2;
    }

    set_cursor(offset);
}

void print_int(int num) {
    char* buffer;
    int_to_string(num, buffer);
    print_string(buffer);
}

void print_nl() {
    int newOffset = move_offset_to_new_line(get_cursor());
    if (newOffset >= MAX_ROWS * MAX_COLS * 2) {
        newOffset = scroll_ln(newOffset);
    }
    set_cursor(newOffset);
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

void print_backspace() {
    int newCursor = get_cursor() - 2;
    set_char_at_video_memory(' ', newCursor);
    set_cursor(newCursor);
}

void print_hex(u8 num) {
    int buffer;
    buffer = hex_to_int(num);
    print_int(buffer);
}
