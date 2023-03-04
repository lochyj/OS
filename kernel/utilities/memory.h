#include <stdint.h>
#include "include/display.h"
#include "kernel/inc/util.h"

#ifndef MEMORY_H

#define MEMORY_H

typedef struct dynamic_mem_node {
    u32 size;
    bool used;
    struct dynamic_mem_node *next;
    struct dynamic_mem_node *prev;
} dynamic_mem_node;

#define DYNAMIC_MEM_TOTAL_SIZE 4*1024
#define DYNAMIC_MEM_NODE_SIZE sizeof(dynamic_mem_node)

static u8 dynamic_mem_area[DYNAMIC_MEM_TOTAL_SIZE];
static dynamic_mem_node *dynamic_mem_start;

void print_dynamic_node_size() {
    char node_size_string[256];
    int_to_string(DYNAMIC_MEM_NODE_SIZE, node_size_string);
    print_string("DYNAMIC_MEM_NODE_SIZE = ");
    print_string(node_size_string);
    print_nl();
}

void init_dynamic_mem() {
    dynamic_mem_start = (dynamic_mem_node *) dynamic_mem_area;
    dynamic_mem_start->size = DYNAMIC_MEM_TOTAL_SIZE - DYNAMIC_MEM_NODE_SIZE;
    dynamic_mem_start->next = NULL_POINTER;
    dynamic_mem_start->prev = NULL_POINTER;
}

void print_dynamic_mem_node(dynamic_mem_node *node) {
    char size_string[256];
    int_to_string(node->size, size_string);
    print_string("{size = ");
    print_string(size_string);
    char used_string[256];
    int_to_string(node->used, used_string);
    print_string("; used = ");
    print_string(used_string);
    print_string("}; ");
}

void print_dynamic_mem() {
    dynamic_mem_node *current = dynamic_mem_start;
    print_string("[");
    while (current != NULL_POINTER) {
        print_dynamic_mem_node(current);
        current = current->next;
    }
    print_string("]\n");
}

void *find_best_mem_block(dynamic_mem_node *dynamic_mem, size_t size) {
    dynamic_mem_node *best_mem_block = (dynamic_mem_node *) NULL_POINTER;    // Initialize the result pointer with NULL and an invalid block size
    u32 best_mem_block_size = DYNAMIC_MEM_TOTAL_SIZE + 1;                   // This is larger than the maximum possible size of the dynamic memory area and will be overwritten by the first valid block

    dynamic_mem_node *current_mem_block = dynamic_mem;
    while (current_mem_block) {
        if ((!current_mem_block->used) && (current_mem_block->size >= (size + DYNAMIC_MEM_NODE_SIZE)) && (current_mem_block->size <= best_mem_block_size)) {
            best_mem_block = current_mem_block;             // This is the best block, update it.
            best_mem_block_size = current_mem_block->size;  // Same here.
        }
        current_mem_block = current_mem_block->next;        // Move to the next block.
    }
    return best_mem_block;
}

void *malloc(size_t size) {
    dynamic_mem_node *best_mem_block = (dynamic_mem_node *) find_best_mem_block(dynamic_mem_start, size);

    if (best_mem_block != NULL_POINTER) {                                           // Check if we found matching 
        best_mem_block->size = best_mem_block->size - size - DYNAMIC_MEM_NODE_SIZE; // Subtract the newly allocated memory including the size of the memory node from the selected block

        dynamic_mem_node *mem_node_allocate = (dynamic_mem_node *) ((u8 *) best_mem_block + DYNAMIC_MEM_NODE_SIZE + best_mem_block->size); // Create the new mem node after the selected node, this effectively splits the region into two.

        mem_node_allocate->size = size;                 // Set the size of the new mem node
        mem_node_allocate->used = true;                 // Set the used flag of the new mem node
        mem_node_allocate->next = best_mem_block->next; // Set the next pointer of the new mem node
        mem_node_allocate->prev = best_mem_block;       // Set the prev pointer of the new mem node

        if (best_mem_block->next != NULL_POINTER) {     // Check if the selected block has a next block
            best_mem_block->next->prev = mem_node_allocate; // Set the prev pointer of the next block to the new mem node
        }

        best_mem_block->next = mem_node_allocate;       // Set the next pointer of the selected block to the new mem node

        return (void *) ((u8 *) mem_node_allocate + DYNAMIC_MEM_NODE_SIZE); // Return the pointer to the newly allocated memory
    }
    return NULL_POINTER;
}

void *merge_next_node_into_current(dynamic_mem_node *current_mem_node) {
    dynamic_mem_node *next_mem_node = current_mem_node->next;
    if (next_mem_node != NULL_POINTER && !next_mem_node->used) {
        current_mem_node->size += current_mem_node->next->size; // Add the size of the next block to the current block
        current_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        current_mem_node->next = current_mem_node->next->next;  // Remove the next block from the list
        if (current_mem_node->next != NULL_POINTER) {
            current_mem_node->next->prev = current_mem_node;
        }
    }
    return current_mem_node;
}

void *merge_current_node_into_prev(dynamic_mem_node *current_mem_node) {
    dynamic_mem_node *prev_mem_node = current_mem_node->prev;
    if (prev_mem_node != NULL_POINTER && !prev_mem_node->used) {
        // add size of previous block to current block
        prev_mem_node->size += current_mem_node->size;
        prev_mem_node->size += DYNAMIC_MEM_NODE_SIZE;

        // remove current node from list
        prev_mem_node->next = current_mem_node->next;
        if (current_mem_node->next != NULL_POINTER) {
            current_mem_node->next->prev = prev_mem_node;
        }
    }
    return prev_mem_node;
}


void mem_free(void *ptr) {
    if (ptr == NULL_POINTER) { // Check if the pointer is NULL
        return; // Return if it is
    }

    dynamic_mem_node *mem_node_free = (dynamic_mem_node *) ((u8 *) ptr - DYNAMIC_MEM_NODE_SIZE); // Get the mem node of the pointer

    if (mem_node_free == NULL_POINTER) { // Check if the mem node is NULL
        return; // Return if it is
    }

    mem_node_free->used = false; // Set the used flag of the mem node to false

    mem_node_free = merge_next_node_into_current(mem_node_free); // Merge the next node into the current node if possible
    merge_current_node_into_prev(mem_node_free); // Merge the current node into the previous node if possible
}

void* alloc(int n) {
    int *ptr = (int *) malloc(n * sizeof(int));
    if (ptr == NULL_POINTER) {
        print_string("Memory not allocated.\n");
    }
    return ptr;
}

#endif