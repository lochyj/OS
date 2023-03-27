#pragma once

int* kprintf_number(int* argp, int length, int sign, int radix);
void kprintf(const char* format, ...);

#include "libraries/kernel/kprintf.c"