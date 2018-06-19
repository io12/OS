#ifndef _KPRINTF_H_
#define _KPRINTF_H_

#include <ints.h>
#include <klibc.h>

typedef enum {
	PL_ALL,
	PL_FRAMEBUFFER,
	PL_SERIAL,
} PrintLocation;

void kprint_udec(PrintLocation pl, u32 num);
void kprint_uhex(PrintLocation pl, u32 num);
void kprintf(PrintLocation pl, const char* fmt, ...)
	__attribute__((format (printf, 2, 3)));
void kvprintf(PrintLocation pl, const char* fmt, va_list ap)
	__attribute((format (printf, 2, 0)));
void udec_to_str(char* buf, u32 num);
void uhex_to_str(char* buf, u32 num);

#endif
