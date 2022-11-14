#include "inc/types.h"
#include "inc/util.h"
#include "libs/registry/registry.h"

#ifndef SHELL_H
#define SHELL_H

/**  
* A function that parses and executes commands for the shell, usually used when getting the users input.
* @param input The input string to be parsed and executed
*/
void execute_command(char* input) {

    // TODO: seperate the input into command and arguments
    // int i = 0;
    // while (input[i] != ' ' && input[i] != '\0') {
    //     i++;
    // }

    // char* command = '\0';

    // for (int j = 0; j < i; j++) {
    //     command[j] = input[j];
    // }
    // ----

    int hit = false;


    for (int i = 0; i < registry_entry_count; i++) {
        if (get_registry_entry(i).type == 0) {
            if (compare_string(get_registry_entry(i).value, input) == false) {
                print_string("Command fount at registry entry ");
                char* loc; int_to_string(i, loc);
                print_string(loc);
                print_nl();
                hit = true;
            }
        }
    }

    if (compare_string(input, "EXIT") == false) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } 
    
    else if (compare_string(input, "CLS") == false) {
        clear_screen();
        hit = true;
    } 
    
    else if (compare_string(input, "HELP") == false) {

        print_string("Available commands:\n");
        for (int i = 0; i < registry_entry_count; i++) {
            if (get_registry_entry(i).type == 0) {
                print_string(get_registry_entry(i).value);
                print_nl();
            }
        }
        print_string("EXIT\nCLS\nHELP\n");
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

