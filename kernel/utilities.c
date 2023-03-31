#include "system.h"

// TODO: Move this to stdlib

void* memcpy(void* dest, const void* src, int count) {
	unsigned char* destC = (unsigned char*)dest;
	unsigned char* srcC = (unsigned char*)src;
	int i;
	for (i = 0; i < count; i++)
		destC[i] = srcC[i];
	return dest;
}

void* memset(void* dest, unsigned char val, int count) {
	unsigned char* destC = (unsigned char*)dest;
	int i;
	for (i = 0; i < count; i++)
		destC[i] = val;
	return dest;
}

unsigned short* memsetw(unsigned short* dest, unsigned short val, int count) {
	int i;
	for (i = 0; i < count; i++)
		dest[i] = val;
	return dest;
}

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

int hex_to_int(uint8_t hex) {
    int n = (int) hex;
    return n;
}

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

int strlen(const char *str) {
	const char *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
}
