#pragma once

#include <stdint.h>

// Info on the pointer and its use. https://wiki.osdev.org/Printing_To_Screen
uint16_t *textmemptr = (uint16_t *)0xB8000;
int text_color = 0x0F;

// Cursor x and y positions
int csr_x = 0, csr_y = 0;

void kprint(char* str);