#pragma once

/* This defines what the stack looks like after an ISR was running */
typedef struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} regs_t;

void isr_install();

void _fault_handler(regs_t r);

#include "kernel/system/interrupt/isr.c"