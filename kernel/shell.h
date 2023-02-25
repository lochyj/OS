#include "kernel/inc/util.h"
#include "kernel/utilities/time.h"

#include <stdint.h>
#include <stdio.h>

#ifndef SHELL_H
#define SHELL_H

//#define Function (...) void *FullFunction(0, POP_LAST(__VA_ARGS__))

typedef void (*Function) (char* input);

typedef struct {
    char* name;
    Function functionCall;
} Command;

Command commandList[10];
int commandListLength = 0;

int execCommands(char* input) {

    if (commandListLength == 0) return 0;

    char command[25];

    for (int i = 0; i < string_length(input); i++) {
        // TODO: Future error handling can start here
        if (i > 25) return 0;
        if (compare_char(input[i], ' ') == 0) {
            command[i] = '\0';
            break;
        }
        command[i] = input[i];
    }

    for (int i = 0; i < commandListLength; i++) {
        if (compare_string(commandList[i].name, command) == 0) {
            commandList[i].functionCall(input);
            return 1;
        }
    }

    // Found nothing
    return 0;
}

void execute_shell_input(char* input) {
    int found = false;

    input = to_upper_string(input);

    if (compare_string(input, "EXIT") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (compare_string(input, "CLS") == 0) {
        clear_screen();
        found = true;
    } else if (compare_string(input, "HELP") == 0) {
        print_string("Available commands: EXIT, CLS, HELP\n");
        found = true;
    } else if (compare_string(input, "TIME") == 0) {
        print_time(get_kernel_time());
        found = true;
    }

    int execReturn = execCommands(input);

    if (execReturn == 1) {
        found = true;
    }

    // If the command was not recognized, print an error message.
    if (found == false) {
        printf("Unknown command '%s' Type HELP for a list of available commands.\n", input);
    }

    found = false;
    printf("> ");
}

#endif