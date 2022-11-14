#include "../../kernel/libs/registry/shell_registry.h"
#include "../../kernel/drivers/keyboard.h"
#include "../../kernel/drivers/vga.h"

#ifndef EDITOR_H

#define EDITOR_H

struct editor_line {
    char* line;
    int line_num;
};

void editor_command() {
    print_string("You called the editor command!\n");
    // Get the users input
}

#endif