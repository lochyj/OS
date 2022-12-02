#include "kernel/inc/types.h"
#include "kernel/inc/util.h"
#include "kernel/utilities/time.h"

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

//! This is not working at the moment...

// #include "kernel/inc/types.h"
// #include "kernel/inc/util.h"
// #include "kernel/utilities/time.h"
// #include "kernel/utilities/memory.h"

// #ifndef SHELL_H
// #define SHELL_H

// typedef void (*ShellFunction) (int argc, char* argv[]);

// typedef struct shell_command {
//     char* name;         // Name of the command
//     int type;           // 0 = internal, 1 = external;
//     char* value;        // The value used for matching and calling the function in the command line / shell
//     char* description;  // The description of the command which will be displayed when the user types help
//     ShellFunction callback[1]; // Function pointer
    
// } shell_command;

// typedef struct shell_registry {
//     shell_command* commands[1001];
//     int command_count;
//     bool enabled;
// } shell_registry;

// static const int kshell_reg_type = 0;
// static const int non_kshell_reg_type = 1;

// // --- Init shell commands registry ---
// shell_command commands[1001];

// shell_registry shell_reg = {
//     .commands = commands,
//     .command_count = 0,
//     .enabled = true
// };

// int shell_commands_length = 0;
// // --- End ---

// // --- Commands ---

// void exit(int argc, char* argv[]) {
//     print_string("Stopping the CPU. Bye!\n");
//     asm volatile("hlt");
// }

// void cls(int argc, char* argv[]) {
//     clear_screen();
// }

// void help(int argc, char* argv[]) {
//     print_nl();
//     print_string(" --- HELP --- ");
//     print_nl();
//     for (int i = 0; i <= shell_commands_length; i++) {
//         print_string(commands[i].value);
//         print_string(" : ");
//         print_string(commands[i].name);
//         print_string(" ");
//         print_string(commands[i].description);
//         print_nl();
//     }
//     print_string(" --- HELP --- ");
//     print_nl();
// }

// // --- End ---

// // --- Admin and init_kshell ---
// void add_kshell_command(shell_command command) {
//     for (int i = 0; i < shell_commands_length; i++) {
//         if (i == (shell_commands_length - 1) ) {
//             /*shell_reg.*/commands[i] = command;
//         }
//     }
//     shell_commands_length++;
// }

// //! this is causing issues
// void init_kshell() {
//     ShellFunction exit_function = &exit;
//     shell_command exit_command = {"Exit", 0, "exit", "Stops the cpu", {&exit}};
//     add_kshell_command(exit_command);

//     ShellFunction cls_function = &cls;
//     shell_command cls_command = {"Clear screen", 0, "cls", "Clears the screen", {&cls}};
//     add_kshell_command(cls_command);

//     ShellFunction help_function = &help;
//     shell_command help_command = {"Help", 0, "help", "Displays this help menu", {&help}};
//     add_kshell_command(help_command);
// }

// void add_shell_command(shell_command command) {
//     //command.type = non_kshell_reg_type; // This causes a bug, so I'm not using it.

//     for (int i = 0; i < shell_commands_length; i++) {
//         if (i == (shell_commands_length - 1) ) {
//             /*shell_reg.*/commands[i] = command;
//         }
//     }
//     shell_commands_length++;
// }

// // --- This is called by the keyboard driver ---
// // TODO: add a check for the keyboard driver that checks if the kernel wants to call shell commands
// void execute_shell_input(char* input) {
//     bool hit = false;

//     print_string("Command starting value: ");
//     print_string(commands[1].value);
//     print_nl();

//     int argc = 0;
//     // NOTE: -possible sources of errors- * Is a source of errors // TODO: fix the errors lol
//     char** argvt = alloc(1000);

//     int j = 0;
//     for (int i = 0; i < string_length(input); i++) {
//         if (input[i] == ' ') {
//             argc++; i++; j = 0;
//         }
//         argvt[argc][j] = input[i];
//         j++;
//     }

//     char** argv = argvt;

//     mem_free(argvt);

//     print_string(argv[0]);

//     print_string("\nA\n");

//     // Increase the size of argc by 1 and make that location in argvt a null pointer to be standard with all other kernels.
//     argc++;
//     ////argvt[argc] = NULL_POINTER;

//     for (int i = 0; i <= shell_commands_length; i++) {
//         print_string("Command value: ");
//         print_string(commands[i].value);
//         print_nl();
//         if (compare_string(commands[i].value, argv[0]) == 0) {  // This is activated even if its not the same?? what?
//             hit = true;
//             commands[i].callback[0](argc, argv);
//             break;
//         }
//     }

//     print_string("\nA\n");

//     // Checks if there was a command hit and if not, prints an error message.
//     if(argv[0] != NULL_POINTER) {
//         if (hit == false) {
//             print_string("Unknown command '"); print_string(argv[0]); print_string("' Type HELP for a list of available commands.\n");
//         }
//     }
//     else {
//         if (hit == false) {
//             print_string("Command wasn't found; Type HELP for a list of available commands.\n");
//         }
//     }

//     print_string("\nA\n");

//     ////print_dynamic_mem(argv);

//     //mem_free(argv);

//     hit = false;
//     print_string("> ");
// }

// #endif
