#pragma once

#define VGA_ADDR 0xB8000
#define VGA_COLOUR 0x0F

#define VGA_WIDTH 80

uint16_t x = 0;
uint16_t y = 0;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);
uint16_t get_cursor_position(void);

void kputc(char c);
void kprint_s(const char* string);

#include "libraries/display.c"
