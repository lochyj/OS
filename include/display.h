#pragma once

#include <stdint.h>

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 15
#define VGA_OFFSET_HIGH 14

void set_cursor(int x, int y);

u16 get_cursor();

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25             //* 25 x Characters
#define MAX_COLS 80             //* 80 x Characters

#define VGA_WIDTH 80            //* 80 x Characters

// 0x0f is WHITE text on a BLACK background
// Current display color
char video_color = 0x0F;

void set_char_at_video_memory(char character, u16 offset);

int get_x_from_offset(int offset);
int get_y_from_offset(int offset);

u16 get_offset(int x, int y);
u16 move_offset_to_new_line(int x, int y);

u16 scroll_ln(int x, int y);

void print_string(char *string);

void putc(char chr);

void print_int(int num);

void print_nl();

void clear_screen();

void print_backspace();

void print_hex(u8 num);

#include "kernel/drivers/vga.c"
