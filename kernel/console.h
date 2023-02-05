#include "kernel/drivers/vga.h"

#ifndef CONSOLE_H

#define CONSOLE_H

void writeLine(int linenum, char* line) {
    int offset = get_cursor();
    set_cursor(MAX_COLS * linenum);
    print_string(line);
    set_cursor(offset);
}

#endif