/* entry point of our kernel */
ENTRY(_start)

SECTIONS {
	/* we need atleast 1MB of space */
	. = 1M;
	kernel_base = .;
  	/* text section */
	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.text)
	}

	/* read only data section */
	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}

	/* data section */
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}

	/* bss section */
	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}
	kernel_end = .;
}