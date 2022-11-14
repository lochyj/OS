
#ifndef S_REGISTRY_H

#define S_REGISTRY_H

typedef void (*FunctionCallback)(int);

struct shell_registry_entry {
    char* name;
    char* value;
    int type;
    FunctionCallback funCall;
};

struct shell_registry_entry registry[100];

int registry_entry_count = 0;

// TODO: implement a proper way of adding keys to registry entries
void add_registry_entry(char* name, char* value, int type, FunctionCallback funCall) {
    registry[registry_entry_count].name = name;
    registry[registry_entry_count].value = value;
    registry[registry_entry_count].type = type;
    registry[registry_entry_count].funCall = funCall;
    registry_entry_count++;
}

struct shell_registry_entry get_shell_registry_entry(int index) {
    return registry[index];
}
#endif
