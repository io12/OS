/* kprintf.h */

#ifndef _KPRINTF_H_
#define _KPRINTF_H_

void kprint_dec(int num);
void kprintf(const char* fmt, ...) __attribute__((format (printf, 1, 2)));

#endif
