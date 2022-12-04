#ifndef STDINT_H

#define STDINT_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef u64 size_t;

typedef u8 bool;

#define false       0
#define true        1

#define NULL_POINTER    ((void*)0)
#define NULL            NULL_POINTER
#define NUL             '\0'
#define min(a,b)        ((a) < (b) ? (a) : (b))
#define max(a,b)        ((a) > (b) ? (a) : (b))

#endif
