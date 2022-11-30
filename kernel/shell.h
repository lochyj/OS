#include "inc/types.h"
#include "inc/util.h"

#ifndef SHELL_H
#define SHELL_H

// struct shell_command {
//     char* name;         // Name of the command
//     int type;           // 0 = internal, 1 = external;
//     char* value;
//     void (*function)(char* input); // function pointer
    
// };

// void exit(char* input) {
//     print_string("Stopping the CPU. Bye!\n");
//     asm volatile("hlt");
// }

void execute_shell_input(char* input) {
    int hit = false;
    if (compare_string(input, "EXIT") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (compare_string(input, "CLS") == 0) {
        clear_screen();
        hit = true;
    } else if (compare_string(input, "HELP") == 0) {
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

