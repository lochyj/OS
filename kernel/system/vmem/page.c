#include "kernel/system/vmem/page.h"
#include "kernel/system/vmem/memory.h"

page_directory_t* current_directory = 0;
page_directory_t* kernel_directory = 0;

uint32_t *frames;
uint32_t nframes;

// Defined in kernel/system/vmem/memory.c
extern uint32_t placement_address;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32_t frame_addr) {
   uint32_t frame =  frame_addr / 0x1000;
   uint32_t idx   =  INDEX_FROM_BIT(frame);
   uint32_t off   =  OFFSET_FROM_BIT(frame);
   frames[idx]    |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr) {
   uint32_t frame = frame_addr / 0x1000;
   uint32_t idx   = INDEX_FROM_BIT(frame);
   uint32_t off   = OFFSET_FROM_BIT(frame);
   frames[idx]    &= ~(0x1 << off);
}

static uint32_t first_frame() {
    uint32_t i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        // Nothing free, exit early.
        if (!(frames[i] != 0xFFFFFFFF)) continue;

        // At least one bit is free here.
        for (j = 0; j < 32; j++) {
            uint32_t toTest = 0x1 << j;
            if (!(frames[i] & toTest)) {
                return i * 4 * 8 + j;
            }
        }
    }

    return (addr) - 1;
} 

void alloc_frame(page_t *page, int permissions, int is_writeable) {
    if (page->frame != 0) {
        // Frame was already allocated, return;
        return;
    }

    else {
        uint32_t idx = first_frame(); // idx is now the index of the first free frame.
        if (idx == (uint32_t)-1) 
            PANIC("No free frames!");

        set_frame(idx * 0x1000);                   // This frame is now the frame we want
        page->present   = 1;                       // Mark it as present.
        page->rw        = (is_writeable)  ? 1 : 0; // Should the page be writeable?
        page->user      = (permissions)   ? 0 : 1; // Should the page be user-mode? - True for kernel, false for user
        page->frame     = idx;
    }
}

void free_frame(page_t *page) {
    uint32_t frame;

    // The page hasn't been allocated. return;
    if (!(frame = page->frame))
        return;

    clear_frame(frame); // Frame is now free again.
    page->frame = 0x0;  // Page now doesn't have a frame.
}

void initialise_paging() {
    // The size of physical memory. For the moment we assume it is 16MB big.
    uint32_t mem_end_page = 0x1000000;

    nframes     = mem_end_page / 0x1000;
    frames      = (uint32_t*) kmalloc(INDEX_FROM_BIT(nframes));

    memset(frames, 0, INDEX_FROM_BIT(nframes));

    // Create a page directory.
    kernel_directory    = (page_directory_t*) kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    current_directory   = kernel_directory;

    int i = 0;
    while (i < placement_address) {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    // Register the page fault handler.
    irq_install_handler(14, page_fault);

    switch_page_directory(kernel_directory);
}


void switch_page_directory(page_directory_t *dir) {
    uint32_t cr0;
    current_directory = dir;
    __asm__ __volatile__("mov %0, %%cr3":: "r"(dir->tablesPhysical));
    __asm__ __volatile__ ("mov %%cr0, %0;" "or $0x80000000, %0;" "mov %0, %%cr0;" : "=r"(cr0) :: "memory"); //!WARN: This is what is causing the system to fail.
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir) {
    // Turn the address into an index.
    address /= 0x1000;

    // Find the page table assigned to this address.
    uint32_t table_idx = address / 1024;

    // If this table is already assigned 
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address%1024];
    } else if(make) {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*) kmalloc_ap(sizeof(page_table_t), &tmp);

        memset(dir->tables[table_idx], 0, 0x1000);

        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.

        return &dir->tables[table_idx]->pages[address%1024];
    }

    return 0;
} 

void page_fault(struct regs* reg) {
    uint32_t faulting_address;

    // The faulting address is stored in the CR2 register. Retrieve it.
    asm ("mov %%cr2, %0" : "=r" (faulting_address)); // This doesn't need to be volatile because it is only retrieving the value of the register.

    // The error code gives us details of what happened.
    int present  = !(reg->err_code & 0x1); // Page not present
    int rw       = reg->err_code & 0x2;    // Write operation?
    int us       = reg->err_code & 0x4;    // Processor was in user-mode?
    int reserved = reg->err_code & 0x8;    // Overwritten CPU-reserved bits of page entry?
    int id       = reg->err_code & 0x10;   // Caused by an instruction fetch?

    kprintf("Page fault occurred.\nInfo: present: %d; r/w: %d; u/s: %d; reserved: %d; i/d: %d; faulting_address: 0x%x;\n", present, rw, us, reserved, id, faulting_address);

    PANIC("Page fault occurred!");
}
