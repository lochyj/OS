#include "blink/blink.h"

// Created by Robert Jan Schaper: https://groups.google.com/g/comp.lang.c++.moderated/c/ihafayWmWU8/m/zNFNsvzIxAAJ
char* itoa(int val, int base) {
	static char buf[32] = {0};
	
	int i = 30;
	
	for (; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}
