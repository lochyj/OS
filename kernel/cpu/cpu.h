#include <stdint.h>
#include "kernel/inc/util.h"

#include "include/display.h"
#include "include/ports.h"

#ifndef CPU_H

#define CPU_H

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
/* IRQ definitions */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#define IDT_ENTRIES 256


/* -------------------------------------------------------------------------------------------------------------------------------
* IDT       typedef struct idt_gate_t
*
* Offset    -> The 32 bit offset that represents the memory address of the function that is called when the interrupt is triggered
*               -> Calls the interrupt handler function
* Selector  -> The 16 bit selector of the code segment to jump to when invoking the handler
*               -> The code segment is the segment that the kernel is in.
* Type      -> This is 3 bits that indicate the type of gate
*               -> The gate we will be using is the Interrupt Gate or Gate `110`
* D         -> This is a 1 bit flag that indicates the size of the pointer
*               -> `0` for 16 bits and `1` for 32 bits
* DPL       -> This is a 2 bit field that indicates the privilege level of the caller
*               -> `00` for kernel mode and `11` for user mode
* P         -> This is a 1 bit flag that indicates if the gate is active.
*               -> `0` for inactive and `1` for active
* 0         -> This is a 1 bit flag that is always set to 0
*               -> this is a reserved bit
* -----------------------------------------------------------------------------------------------------------------------------*/

typedef struct {
    u16 low_offset;
    u16 selector;
    u8 always0;
    u8 flags;
    u16 high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    u32 ds;                                         // Data segment selector
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;     // Pushed by pusha
    u32 int_no, err_code;                           // Interrupt number and error code (if applicable)
    u32 eip, cs, eflags, useresp, ss;               // Pushed by the processor automatically
} registers_t;

typedef void (*isr_t)(registers_t*);

// ---------------------------------------------------------
// Loading the IDT  <- IDT is the Interrupt Descriptor Table
// This can be done using the lidt instruction in ASM
// ---------------------------------------------------------

typedef struct {
    u16 limit;      // The upper 16 bits of all selector limits
    u32 base;       // The address of the first gdt_entry_t struct
} __attribute__((packed)) idt_register_t;

// Already defined in "kernel/inc/util.h"
// #define low_16(address) (u16)((address) & 0xffff)
// #define high_16(address) (u16)(((address) >> 16) & 0xffff)

idt_gate_t idt[256];

void set_idt_gate(int n, u32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].selector = 0x08; // See GDT for explanation of 0x08 | GDT <- Global descriptor table A.K.A GDT.asm under "boot/GDT.asm"
    idt[n].always0 = 0;

    /* 0x8e = 1  00  0  1  110
    *         P  DPL 0  D  Type */
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

// ------------------------------------------
// ISR          <- Interrupt Service Routines
// ------------------------------------------

char* exception_messages[] = {
    "Division by zero",
    "Debug",
    "Non maskable interrupt",
    "Breakpoint",
    "Into detected overflow",
    "Out of bounds",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "Reserved"
};

void isr_handler(registers_t* r) {
    print_string("received interrupt: ");
    char s[3];
    int_to_string(r->int_no, s);
    print_string(s);
    print_nl();
    print_string(exception_messages[r->int_no]);
    print_nl();
}

isr_t interrupt_handlers[256];

void irq_handler(registers_t* r) {
    if (interrupt_handlers[r->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }

    port_byte_out(0x20, 0x20);  // This is the primary EOI (End of Interrupt) command
    if (r -> int_no >= 40) {
        port_byte_out(0xA0, 0x20);  // This is the secondary EOI (End of Interrupt) command
    }
}

idt_register_t idt_reg;

void load_idt() {
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_reg));
}

void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_install() {
    set_idt_gate(0, (u32)isr0); // Division by zero
    set_idt_gate(1, (u32)isr1); // Debug
    set_idt_gate(2, (u32)isr2); // Non maskable interrupt
    set_idt_gate(3, (u32)isr3); // Breakpoint
    set_idt_gate(4, (u32)isr4); // Into detected overflow
    set_idt_gate(5, (u32)isr5); // Out of bounds
    set_idt_gate(6, (u32)isr6); // Invalid opcode
    set_idt_gate(7, (u32)isr7); // No coprocessor
    set_idt_gate(8, (u32)isr8); // Double fault
    set_idt_gate(9, (u32)isr9); // Coprocessor segment overrun
    set_idt_gate(10, (u32)isr10); // Bad TSS
    set_idt_gate(11, (u32)isr11); // Segment not present
    set_idt_gate(12, (u32)isr12); // Stack fault
    set_idt_gate(13, (u32)isr13); // General protection fault
    set_idt_gate(14, (u32)isr14); // Page fault
    set_idt_gate(15, (u32)isr15); // Unknown interrupt
    set_idt_gate(16, (u32)isr16); // Coprocessor fault
    set_idt_gate(17, (u32)isr17); // Alignment check
    set_idt_gate(18, (u32)isr18); // Machine check
    set_idt_gate(19, (u32)isr19); // Reserved
    set_idt_gate(20, (u32)isr20); // Reserved
    set_idt_gate(21, (u32)isr21); // Reserved
    set_idt_gate(22, (u32)isr22); // Reserved
    set_idt_gate(23, (u32)isr23); // Reserved
    set_idt_gate(24, (u32)isr24); // Reserved
    set_idt_gate(25, (u32)isr25); // Reserved
    set_idt_gate(26, (u32)isr26); // Reserved
    set_idt_gate(27, (u32)isr27); // Reserved
    set_idt_gate(28, (u32)isr28); // Reserved
    set_idt_gate(29, (u32)isr29); // Reserved
    set_idt_gate(30, (u32)isr30); // Reserved
    set_idt_gate(31, (u32)isr31); // Reserved

    // ----------------------------------------------------------------------------------------------------------------
    // Remapping the PIC    <- The PIC is the Programmable Interrupt Controller
    // The newer and more modern version of the PIC is the APIC which is the Advanced Programmable Interrupt Controller
    // ----------------------------------------------------------------------------------------------------------------

    // Remap the PIC
    port_byte_out(0x20, 0x11);   // ICW1 <- Initialization Command Word 1
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);   // ICW2 <- Initialization Command Word 2
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);   // ICW3 <- Initialization Command Word 3
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);   // ICW4 <- Initialization Command Word 4
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);    // OCW1 <- Mask interrupts
    port_byte_out(0xA1, 0x0);

    // Install the IRQ Handlers
    set_idt_gate(32, (u32)irq0);
    set_idt_gate(33, (u32)irq1);
    set_idt_gate(34, (u32)irq2);
    set_idt_gate(35, (u32)irq3);
    set_idt_gate(36, (u32)irq4);
    set_idt_gate(37, (u32)irq5);
    set_idt_gate(38, (u32)irq6);
    set_idt_gate(39, (u32)irq7);
    set_idt_gate(40, (u32)irq8);
    set_idt_gate(41, (u32)irq9);
    set_idt_gate(42, (u32)irq10);
    set_idt_gate(43, (u32)irq11);
    set_idt_gate(44, (u32)irq12);
    set_idt_gate(45, (u32)irq13);
    set_idt_gate(46, (u32)irq14);
    set_idt_gate(47, (u32)irq15);

    load_idt();
}

#endif