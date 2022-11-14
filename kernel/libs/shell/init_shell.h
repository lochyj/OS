#include "../registry/shell_registry.h"
#include "../../../programs/edit/edit.h"


#ifndef INIT_SHELL

#define INIT_SHELL

void shell_command() {
    print_string("You just called the shell command!\nWhich is stored in the registry!\n");
}
FunctionCallback shell_command_callback = &shell_command;

FunctionCallback editor_command_callback = &editor_command;

void init_shell_commands() {
    add_shell_registry_entry("SHELL", "SHELL", shell_command_callback);
    add_shell_registry_entry("EDITOR", "EDIT", editor_command_callback);
}

#endif
