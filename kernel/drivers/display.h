#pragma once

#include <stdint.h>
#include <string.h>

#include "blink/blink.h"

// cursor.c
void move_csr();
void clear_screen();

// display.c
void putc(char c);
void puts(char *text);
