/* kprintf.h */

#ifndef _KPRINTF_H_
#define _KPRINTF_H_

#include <ints.h>

typedef enum {
	PL_FRAMEBUFFER,
	PL_SERIAL
} PrintLocation;

void udec_to_str(char* buf, u32 num);
void uhex_to_str(char* buf, u32 num);
void kprint_udec(PrintLocation pl, u32 num);
void kprint_uhex(PrintLocation pl, u32 num);
void kprintf(PrintLocation pl, const char* fmt, ...)
	__attribute__((format (printf, 2, 3)));

#endif
