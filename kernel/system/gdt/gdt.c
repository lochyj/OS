#include "kernel/system/gdt.h"

struct gdt_entry gdt[3];
struct gdt_ptr gdt_ptr;

void add_gdt_entry(int index, uint64_t base, uint64_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].base_low = (base & 0xFFFF);
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = (limit & 0xFFFF);
    gdt[index].granularity = ((limit >> 16) & 0x0F);

    gdt[index].granularity |= (granularity & 0xF0);
    gdt[index].access = access;
}

void install_gdt() {
    gdt_ptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    add_gdt_entry(0, 0, 0, 0, 0);
    add_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    add_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    flush_gdt();
}