/* klibc.h */

#ifndef _KLIBC_H_
#define _KLIBC_H_

// stddef.h
#define NULL ((void*) 0)

typedef unsigned long size_t;

// stdbool.h
#define true  1
#define false 0

// string.h
void* memset(void* s, int c, size_t n);
size_t strlen(const char* s);

#endif
