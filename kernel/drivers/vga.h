#include "kernel/drivers/ports.h"
#include "kernel/inc/util.h"

#include <stdint.h>


#ifndef VGA_H

#define VGA_H

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 15
#define VGA_OFFSET_HIGH 14

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;
}

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25             //* 25 x Characters
#define MAX_COLS 80             //* 80 x Characters

u8 video_color = 0x0f;  // 0x0f is WHITE on BLACK

void set_char_at_video_memory(char character, int offset) {
    unsigned char *videoMemory = (unsigned char *) VIDEO_ADDRESS;
    videoMemory[offset] = character;
    videoMemory[offset + 1] = video_color;
}

int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
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

void print_char(char chr) {
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

#endif