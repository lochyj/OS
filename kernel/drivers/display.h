#pragma once

#include <stdint.h>
#include <string.h>

#include "blink/blink.h"

extern int cursor_x, cursor_y;
extern uint8_t interfaceColours;
extern uint16_t *textmemptr;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

 

// cursor.c
void move_csr();
void clear_screen();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

// display.c
void putc(char c);
void puts(char *text);
void initialise_textmode_terminal();
