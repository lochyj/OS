#include "../inc/types.h"
#include "../inc/util.h"

#ifndef REGISTRY_H

#define REGISTRY_H

typedef void (*FunctionCallback) (int);

typedef struct {

    char* name;

    FunctionCallback* callback;

} SubscribeBus;

typedef struct {

    char* name;

    char* key;

} RegistryKey;

typedef struct {

    char* name;

    int type;

    // NOTE: This will be a problem in the future. I need to make this static in the future.
    RegistryKey keys[10];

} RegistryEntry;

u32 registry_count = 0;

RegistryEntry registry[1001];

bool registry_add_entry(RegistryEntry entry) {
    if (registry_count >= 1000) {
        return false;
    }
    registry[registry_count] = entry;
    registry_count++;
    return true;
}

bool subscribe_to_bus(SubscribeBus bus[], SubscribeBus entry) {
    for (int i = 0; compare_string(bus[i].name, NULL) != 0; i++) {
        bus[i] = entry;
        return true;
    }
    return false;
}

#endif