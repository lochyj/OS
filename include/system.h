#pragma once

#include "types.h"

/* This defines what the stack looks like after an ISR was running */
struct regs {
    uint32_t gs, fs, es, ds;                            /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    /* pushed by 'pusha' */
    uint32_t int_no, err_code;                          /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;              /* pushed by the processor automatically */ 
};

extern void asm_sti();

void* memcpy(void* dest, const void* src, int count);
void* memset(void* dest, unsigned char val, int count);
unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);
char* strcpy(char *to, const char *from);
int strcmp(const char *s1, const char *s2);
unsigned char inb(unsigned short _port);
void outb(unsigned short _port, unsigned char _data);
char* itoa(int val, int base);
int hex_to_int(uint8_t hex);
int strlen(const char *str);

void scroll(void);
void move_csr(void);
void cls();
void putc(char c);
void puts(char* text);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video(void);

void kprintf(const char* format, ...);

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

void isr_install();
void fault_handler(struct regs *r);

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_install();
void irq_handler(struct regs *r);

void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);

void keyboard_handler(struct regs *r);
void keyboard_install();
