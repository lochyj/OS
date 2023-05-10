[bits 32]

global flush_gdt     ; Allows the C code to link to this
extern gdt_ptr            ; Says that 'gp' is in another file
flush_gdt:
    lgdt [gdt_ptr]
    mov ax, 0x10      ; 0x10 is the offset in the GDT to the data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2

flush2:
    ret