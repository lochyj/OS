
#ifndef REGISTRY_H

#define REGISTRY_H

/*  
* This is a basic registry system for the kernel. It is used to store and retrieve data commonly used by the kernel, such as shell commands and other important information.
*/


struct registry_entry {
    char* name;
    char* value;
    int type;
};

struct registry_entry registry[100];

int registry_entry_count = 0;

// TODO: implement a proper way of adding keys to registry entries
void add_registry_entry(char* name, char* value, int type) {
    registry[registry_entry_count].name = name;
    registry[registry_entry_count].value = value;
    registry[registry_entry_count].type = type;
    registry_entry_count++;
}

struct registry_entry get_registry_entry(int index) {
    return registry[index];
}


/**
 * @brief The type of registry entries for easy access.
 * 
 * @param 0 - Kernel infomation
 * 
 * @param 1 - All program locations
 * 
 */
void registry_types() {};
#endif
