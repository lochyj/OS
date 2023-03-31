#pragma once

typedef unsigned long int addr;

/* This defines what the stack looks like after an ISR was running */
struct regs {
    unsigned int gs, fs, es, ds;                            /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;    /* pushed by 'pusha' */
    unsigned int int_no, err_code;                          /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;              /* pushed by the processor automatically */ 
};

// From kernel/boot.asm
extern void asm_sti();

// From kernel/utilities.c
void* memcpy(void* dest, const void* src, int count);
void* memset(void* dest, unsigned char val, int count);
unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);
unsigned char inb(unsigned short _port);
void outb(unsigned short _port, unsigned char _data);
int strlen(const char *str);

// From kernel/display.c
void scroll(void);
void move_csr(void);
void cls();
void putch(char c);
void puts(char *text);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video(void);

// From kernel/gdt.c
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();

// From kernel/idt.c
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

// From kernel/isr.c
void isr_install();
void fault_handler(struct regs *r);

// From kernel/irq.c
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_install();
void irq_handler(struct regs *r);

// From kernel/clock.c
void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);

// From kernel/keyboard.c
void keyboard_handler(struct regs *r);
void keyboard_install();
