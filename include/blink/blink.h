#pragma once

// Utilities.c
unsigned char inb(unsigned short _port);
void outb(unsigned short _port, unsigned char _data);

char* itoa(int val, int base);

int hex_to_int(unsigned char hex);
// -
