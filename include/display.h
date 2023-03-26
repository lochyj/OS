#pragma once

#define VGA_ADDR 0xB8000
#define VGA_COLOUR 0x0F

#define VGA_WIDTH 80

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);
uint16_t get_cursor_position(void);
uint16_t get_offset(uint16_t x, uint16_t y);
uint16_t set_new_line(uint16_t pos);

void kprint_s(char* string);

#include "libraries/display.c"
