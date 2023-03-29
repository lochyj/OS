#pragma once

#include "kernel/system/interrupt/isr.h"

extern void irq_install();
extern void irq_install_handler(int irq, void (*handler)(struct regs* r));
extern void irq_uninstall_handler(int irq);

#include "kernel/system/interrupt/irq.c"
