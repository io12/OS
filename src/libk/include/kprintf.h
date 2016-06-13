/* kprintf.h */

#ifndef _KPRINTF_H_
#define _KPRINTF_H_

typedef enum {
	PL_FRAMEBUFFER,
	PL_SERIAL
} PrintLocation;

void dec_to_str(char* buf, int num);
void kprint_dec(PrintLocation pl, int num);
void kprintf(PrintLocation pl, const char* fmt, ...)
	__attribute__((format (printf, 2, 3)));

#endif
