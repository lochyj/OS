#ifndef SHELL_H
#define SHELL_H

void execute_command(char *input) {
    if (compare_string(input, "EXIT") == 0) {
        print_string("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    print_string("Unknown command: ");
    print_string(input);
    print_string("\n> ");
}

#endif

