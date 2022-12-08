//TODO: move this to the utilities folder for a more general use.

#include <stdint.h>
#include "../inc/util.h"

#ifndef REGISTRY_H

#define REGISTRY_H

// TODO: remove this entirely
typedef void (*FunctionCallback) (u8);

typedef struct SubscribeBus {

    char* name;

    FunctionCallback callback[1];

} SubscribeBus;

void subscribe_to_bus(SubscribeBus bus[], SubscribeBus entry, int *loc) {
    bus[*loc] = entry;
    *loc++;
}

#endif
