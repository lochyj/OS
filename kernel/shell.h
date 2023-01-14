#include "kernel/inc/util.h"
#include "kernel/utilities/time.h"

#include <stdint.h>

#ifndef SHELL_H
#define SHELL_H

void execute_shell_input(char* input) {
    int hit = false;

    input = to_upper_string(input);

    if (compare_string(input, "EXIT") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (compare_string(input, "CLS") == 0) {
        clear_screen();
        hit = true;
    } else if (compare_string(input, "HELP") == 0) {
        print_string("Available commands: EXIT, CLS, HELP\n");
        hit = true;
    } else if (compare_string(input, "TIME") == 0) {
        print_time(get_kernel_time());
        hit = true;
    }

    // If the command was not recognized, print an error message.
    if (hit == false) {
        print_string("Unknown command '"); print_string(input); print_string("' Type HELP for a list of available commands.\n");
    }

    hit = false;
    print_string("> ");
}

#endif