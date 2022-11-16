#include "../../include.h"

#ifndef S_REGISTRY_H

#define S_REGISTRY_H

struct shell_registry_entry shell_registry[100];

int shell_registry_entry_count = 0;

// TODO: implement a proper way of adding keys to registry entries
void add_shell_registry_entry(char* name, char* value, FunctionCallback funCall) {
    shell_registry[shell_registry_entry_count].name = name;
    shell_registry[shell_registry_entry_count].value = value;
    shell_registry[shell_registry_entry_count].funCall = funCall;
    shell_registry_entry_count++;
}

struct shell_registry_entry get_shell_registry_entry(int index) {
    return shell_registry[index];
}

#endif
