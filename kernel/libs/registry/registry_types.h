
#ifndef REGISTRY_TYPES_H

#define REGISTRY_TYPES_H

typedef void (*FunctionCallback)(int);

struct keys {
    char* name;
    char* value;
};

struct subscription_bus {
    char* name;
    FunctionCallback callback;
};

struct shell_registry_entry {
    char* name;
    char* value;
    FunctionCallback funCall;
};

struct registry_entry {
    char* name;
    char* value;
    int type;
};

#endif