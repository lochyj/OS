#pragma once

#include "libraries/port.c"

unsigned char inb(unsigned short port);

void outb(unsigned short port, unsigned char data);
