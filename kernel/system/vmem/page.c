#include "kernel/system/vmem/page.h"
#include "kernel/system/vmem/memory.h"

page_directory_t* current_directory = 0;
page_directory_t* kernel_directory = 0;

// A bitset of frames - used or free.
uint32_t *frames;
uint32_t nframes;

extern uint32_t placement_address;

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr) {
   uint32_t frame   = frame_addr/0x1000;
   uint32_t idx     = INDEX_FROM_BIT(frame);
   uint32_t off     = OFFSET_FROM_BIT(frame);
   frames[idx]      |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr) {
   uint32_t frame   = frame_addr/0x1000;
   uint32_t idx     = INDEX_FROM_BIT(frame);
   uint32_t off     = OFFSET_FROM_BIT(frame);
   frames[idx]      &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr) {
   uint32_t frame   = frame_addr/0x1000;
   uint32_t idx     = INDEX_FROM_BIT(frame);
   uint32_t off     = OFFSET_FROM_BIT(frame);

   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame() {
    uint32_t i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        // nothing free, exit early. 
        if (frames[i] != 0xFFFFFFFF){
            // at least one bit is free here.
            for (j = 0; j < 32; j++) {
                uint32_t toTest = 0x1 << j;
                if ( !(frames[i] &toTest) ) {
                    return i * 4 * 8 + j;
                }
            }
        }
    }
    return -1;
}

// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
    if (page->frame != 0) {
        // Frame was already allocated, return straight away.
        return;
    }
    else {
        uint32_t idx = first_frame(); // idx is now the index of the first free frame.
        if (idx == (uint32_t)-1) {
            PANIC("No free frames!");
        }
        set_frame(idx*0x1000); // this frame is now ours!
        page->present   = 1; // Mark it as present.
        page->rw        = (is_writeable)?1:0; // Should the page be writeable?
        page->user      = (is_kernel)?0:1; // Should the page be user-mode?
        page->frame     = idx;
    }
}

void initialise_paging() {
	// The size of physical memory. For the moment we assume it is 16MB big.
	uint32_t mem_end_page = 0x1000000;

	nframes = mem_end_page / 0x1000;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	// Let's make a page directory.
	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;

	/* We need to identity map (phys addr = virt addr) from
	0x0 to the end of used memory, so we can access this
	transparently, as if paging wasn't enabled.*/
	int i = 0;
	while (i < placement_address) {
		// Kernel code is readable but not writeable from userspace.
		alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}
	
	// Before we enable paging, we must register our page fault handler.
    irq_install_handler(14, page_fault);

	// Now, enable paging!
    enable_paging();

	switch_page_directory(kernel_directory);
}

void enable_paging() {
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));

    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
}

page_t* get_page(uint32_t address, int make, page_directory_t *dir) {
	// Turn the address into an index.
	address /= 0x1000;

	// Find the page table containing this address.
	uint32_t table_idx = address / 1024;

	// If this table is already assigned
	if (dir->tables[table_idx]) {
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else if(make) {
		uint32_t tmp;
		dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else
		return 0;
}

void page_fault(struct regs* r) {
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   uint32_t faulting_address;
   asm ("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present      = !(r->err_code & 0x1);     // Page not present
   int rw           = r->err_code & 0x2;        // Write operation?
   int us           = r->err_code & 0x4;        // Processor was in user-mode?
   int reserved     = r->err_code & 0x8;        // Overwritten CPU-reserved bits of page entry?
   int id           = r->err_code & 0x10;       // Caused by an instruction fetch?

   // Output an error message.
   kprintf("Page fault! ( ");
   if (present) {kprintf("present ");}
   if (rw) {kprintf("read-only ");}
   if (us) {kprintf("user-mode ");}
   if (reserved) {kprintf("reserved ");}
   kprintf(") at 0x");
   kprintf("%x", faulting_address);
   kprintf("\n");
   PANIC("Page fault");
} 
