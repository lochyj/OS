; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
global _idt_load
extern idtp
_idt_load:
    lidt [idtp]
    ret
