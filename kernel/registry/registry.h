//TODO: move this to the utilities folder for a more general use.

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

    // INFO: there is a max of 10 keys per entry for size reasons -> 1001 * 10 = 10,010 keys so yeah
    RegistryKey keys[1];

} RegistryEntry;

int registry_count = 0;

RegistryEntry registry[1001];

void registry_add_entry(RegistryEntry entry) {
    registry[registry_count] = entry;
    registry_count++;
}

void subscribe_to_bus(SubscribeBus bus[], SubscribeBus entry, int *loc) {
    bus[*loc] = entry;
    *loc++;
}

#endif
