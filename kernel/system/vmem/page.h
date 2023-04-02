#pragma once

#include "system.h"
#include "debug.h"

// From assemblies/paging.asm
extern void loadPageDir(uint32_t*);
extern void enablePaging();

// Creating a blank page directory
uint32_t page_directory[1024] __attribute__((aligned(4096)));

uint32_t page_table[1024] __attribute__((aligned(4096)));
