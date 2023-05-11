#pragma once

unsigned char inb(unsigned short _port);
void outb(unsigned short _port, unsigned char _data);

void io_wait(void);

char* itoa(int val, int base);