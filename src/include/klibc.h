/* klibc.h */

#ifndef _KLIBC_H_
#define _KLIBC_H_

// stddef.h
#define NULL ((void*) 0)

typedef unsigned int size_t;

// stdbool.h
#define true  1
#define false 0

// string.h
int memcmp(const void* p1, const void* p2, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
char* strchr(const char* str, int c);
int strcmp(const char* s1, const char* s2);
size_t strlen(const char* str);

#endif
