#include "inc/types.h"
#include "inc/util.h"

#ifndef SHELL_H
#define SHELL_H

typedef struct shell_command {
    char* name;         // Name of the command
    int type;           // 0 = internal, 1 = external;
    char* value;
    void (*function)(int argc, char* argv[]);   // Function pointer
    
} shell_command;

void exit(char* input) {
    print_string("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
}

shell_command commands = shell_command[1001];

int shell_commands_length = 0;

void execute_shell_input(char* input) {
    int hit = false;

    [aaa, aaa, aaaa]

    int argc = 0;
    char* argv[];

    for (int i = 0; i < string_length(input); i++) {
        if (input[i] == ' ') {
            argc++;
            i++;
        }
        argv[argc] = input[i];
    }

    for (int i = 0; )

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

