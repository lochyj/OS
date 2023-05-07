#include "blink/blink.h"

unsigned char inb(unsigned short _port) {
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

void outb(unsigned short _port, unsigned char _data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

// Created by Robert Jan Schaper: https://groups.google.com/g/comp.lang.c++.moderated/c/ihafayWmWU8/m/zNFNsvzIxAAJ
char* itoa(int val, int base) {
	static char buf[32] = {0};
	
	int i = 30;
	
	for (; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}

int hex_to_int(unsigned char hex) {
    int n = (int) hex;
    return n;
}