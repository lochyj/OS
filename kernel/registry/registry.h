#include "../inc/types.h"
#include "../inc/util.h"

#ifndef REGISTRY_H

#define REGISTRY_H

// TODO: make this a dynamic passthrough, such as "..." or something idk
typedef void (*FunctionCallback) (u8);

typedef struct SubscribeBus {

    char* name;

    FunctionCallback callback[1];

} SubscribeBus;

typedef struct RegistryKey {

    char* name;

    char* key;

} RegistryKey;

typedef struct RegistryEntry {

    char* name;

    int type;

    // TODO: Make this recursive or something
    RegistryKey keys;

} RegistryEntry;

int registry_count = 0;

RegistryEntry registry[1001];

bool registry_add_entry(RegistryEntry entry) {
    if (registry_count >= 1000) {
        return false;
    }
    registry[registry_count] = entry;
    registry_count++;
    return true;
}

void subscribe_to_bus(SubscribeBus bus[], SubscribeBus entry, int loc) {
    bus[loc] = entry;
    loc++;
}

#endif
