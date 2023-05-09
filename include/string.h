#pragma once

void* memcpy(void* dest, const void* src, int count);
void* memset(void* dest, unsigned char val, int count);
unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);
char* strcpy(char *to, const char *from);
int strcmp(const char *s1, const char *s2);
int strlen(const char *str);
