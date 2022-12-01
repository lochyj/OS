#include "inc/types.h"
#include "inc/util.h"
#include "utilities/time.h"

#ifndef SHELL_H
#define SHELL_H

typedef void (*ShellFunction) (int argc, char* argv[]);

typedef struct shell_command {
    char* name;         // Name of the command
    int type;           // 0 = internal, 1 = external;
    char* value;        // The value used for matching and calling the function in the command line / shell
    ShellFunction callback; // Function pointer
    
} shell_command;

// --- Init shell commands registry ---
shell_command commands[1001];

int shell_commands_length = 0;

// --- end ---

// --- commands ---

void exit(int argc, char* argv[]) {
    print_string("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
}

void cls(int argc, char* argv[]) {
    clear_screen();
}

void help(int argc, char* argv[]) {
    // Nothing yet!
}

// --- end ---

// --- admin and init_kshell ---
void init_kshell() {
    ShellFunction exit_function = &exit;
    shell_command exit_command = {"exit", 0, "exit", &exit};

}

void add_shell_command(shell_command command) {
    command.type = 1;

    for (int i = 0; i < shell_commands_length; i++) {
        if (i == (shell_commands_length - 1) ) {
            commands[i] = command;
        }
    }

}


// --- This is called by the keyboard driver ---
// TODO: add a check for the keyboard driver that checks if the kernel wants to call shell commands
void execute_shell_input(char* input) {
    bool hit = false;

    int argc = 0;
    // NOTE: possible sources of errors
    char argv[100][100];

    int j = 0;
    for (int i = 0; i < string_length(input); i++) {
        if (input[i] == ' ') {
            argc++;
            i++;
            j = 0;
            print_nl();
        }
        argv[argc][j] = input[i];
        j++;
    }

    if (compare_string(input, "EXIT") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (compare_string(input, "CLS") == 0) {
        clear_screen();
        hit = true;
    } else if (compare_string(input, "HELP") == 0) {
        print_string("Available commands: EXIT, CLS, HELP\n");
        hit = true;
    } /* else if (compare_string(input, "TIME") == 0) {
        print_time(get_kernel_time());
        hit = true;
    } */

    // If the command was not recognized, print an error message.
    if (hit == false) {
        print_string("Unknown command '"); print_string(input); print_string("' Type HELP for a list of available commands.\n");
    }

    hit = false;
    print_string("> ");
}

#endif
