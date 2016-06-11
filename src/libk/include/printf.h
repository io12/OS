/* printf.h */

#ifndef _PRINT_H_
#define _PRINT_H_

int kprintf(const char* fmt, ...) __attribute__((format (printf, 1, 2)));
int kvprintf(const char* fmt, va_list ap) __attribute__((format (printf, 1, 0)));

#endif
