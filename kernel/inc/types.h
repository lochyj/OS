#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

// This is u64 because it is the same type as the sizeof operator.
typedef u64 size_t;

typedef int bool;

#define true 1
#define false 0

#define NULL_POINTER ((void *)0)
#define NULL = NULL_POINTER
#define NUL '\0'

#endif
