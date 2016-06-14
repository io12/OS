/* kprintf.h */

#ifndef _KPRINTF_H_
#define _KPRINTF_H_

#include <ints.h>

typedef enum {
	PL_FRAMEBUFFER,
	PL_SERIAL
} PrintLocation;

void uint_to_str(char* buf, u32 num);
void kprint_uint(PrintLocation pl, u32 num);
void kprintf(PrintLocation pl, const char* fmt, ...)
	__attribute__((format (printf, 2, 3)));

#endif
