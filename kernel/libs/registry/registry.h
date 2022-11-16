#include "../../include.h"

#ifndef REGISTRY_H

#define REGISTRY_H

/** Registry types table
 * @brief The type of registry entries for easy access.
 * 
 * @param 0 - Kernel data
 * 
 * @param 1 - 
 * 
 * @param 2 - 
 * 
 */

// TODO: make this dynamic in the future
struct registry_entry registry[100];

int registry_entry_count = 0;

// TODO: implement a proper way of adding keys to registry entries
void add_registry_entry(char* name, char* value, int type) {
    registry[registry_entry_count].name = name;
    registry[registry_entry_count].value = value;
    registry[registry_entry_count].type = type;
    registry_entry_count++;
}

struct registry_entry* get_registry_entry_by_name(char* name) {
    struct registry_entry out[100];
    int count = 0;
    for (int i = 0; i < registry_entry_count; i++) {
        if (compare_string(registry[i].name, name) == 0) {
            out[count] = &registry_entry[i];
        }
    }
    count ? return out : return 0;
}

struct registry_entry* get_registry_entry_by_type(int type) {
    struct registry_entry out[100];
    int count = 0;
    for (int i = 0; i < registry_entry_count; i++) {
        if (registry[i].type == type) {
            out[count] = &registry_entry[i];
        }
    }
    count ? return out : return 0;
}

struct registry_entry get_registry_entry_by_index(int index) {
    return registry[index];
}

#endif
