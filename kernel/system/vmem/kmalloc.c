#include "kernel/system/vmem/memory.h"

// End is defined in the linker script.
extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

static uint32_t kmalloc_internal(uint32_t sz, int align, uint32_t *phys) {

    if (align == 1 && (placement_address & 0x00000FFF)) {
        // Align the placement address;
        placement_address &= 0x00000FFF;
        placement_address += 0x1000;
    }

    if (phys) {
        *phys = placement_address;
    }

    uint32_t tmp = placement_address;
    placement_address += sz;

    return tmp;
}

uint32_t kmalloc_a(uint32_t sz) {
    return kmalloc_internal(sz, 1, 0);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys) {
    return kmalloc_internal(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) {
    return kmalloc_internal(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz) {
    return kmalloc_internal(sz, 0, 0);
}