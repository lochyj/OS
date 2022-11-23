#include "inc/types.h"
#include "inc/util.h"

#ifndef SHELL_H
#define SHELL_H

// TODO: these functions
void enable_shell() {

}

void stop_shell() {

}

void shell_input_wrapper() {

}

void execute_command(char *input) {
    int hit = false;
    if (compare_string(input, "EXIT") == false) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (compare_string(input, "CLS") == false) {
        clear_screen();
        hit = true;
    } else if (compare_string(input, "HELP") == false) {
        print_string("Available commands: EXIT, CLS, HELP\n");
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

