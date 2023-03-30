[BITS 32]
global start
start:
    push esp		; Pushs the stack address
	push ebx		; Pushs the 32-bit address of the multiboot info structure to the stack
    jmp stub

; This part MUST be 4byte aligned, thus the ALIGN 4
ALIGN 4
mboot:
    ; Multiboot macros to make a few lines later more readable
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    ; This is the GRUB Multiboot header. A boot signature
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd mboot
    dd code
    dd bss
    dd end
    dd start

stub:
	extern __kernel_main
	cli
	call __kernel_main
    jmp $

; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
global _gdt_flush     ; Allows the C code to link to this
extern gp               ; Says that 'gp' is in another file
_gdt_flush:
    lgdt [gp]        ; Load the GDT with our '_gp' which is a special pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret               ; Returns back to the C code!


; This will refresh the TSS for the processor.
global _tss_flush	; Allows the C code to link to this
_tss_flush:
	mov ax, 0x2B	; Load the index of our TSS structure
			; The index is 0x28 as it is the 5th
			; selector and each is 8 bytes long,
			; but we set the bottom two bits (making
			; 0x2B) so that is has an RPL of 3,
			; not zero.
	ltr ax		; Load 0x2B into the task state register
	ret

; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
global _idt_load
extern idtp
_idt_load:
    lidt [idtp]
    ret


%include "./assemblies/isr.asm"

%include "./assemblies/irq.asm"

SECTION .bss
    resb 8192	; This reserves 8KBytes of memory here
_sys_stack:
