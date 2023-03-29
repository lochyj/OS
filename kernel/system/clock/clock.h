#pragma once

void timer_phase(int hz);

void timer_handler(struct regs *r);

void timer_install();

void timer_wait(int ticks);

#include "kernel/system/clock/clock.c"