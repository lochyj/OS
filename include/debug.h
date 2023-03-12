#pragma once

#include "kernel/debug.c"

static void ERROR(const char* file, int line);

#define ASSERT(expr) do {\
    if (!(expr)) ERROR(__FILE__, __LINE__);\
} while (0)