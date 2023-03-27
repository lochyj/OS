#pragma once

#include "libraries/debug.c"

void __debug_print(const char* string);

char* __debug_itoa(int val, int base);

static void ERROR(const char* file, int line);

#define PANIC(message) do {__DEBUG_PANIC(__FILE__, __LINE__, message);} while (0)

#define ASSERT(expr) do {if (!(expr)) __DEBUG_ERROR(__FILE__, __LINE__);} while (0)