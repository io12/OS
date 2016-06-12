/* arg.h */

// Similar to stdarg.h

#ifndef _ARG_H_
#define _ARG_H_

#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type)   __builtin_va_arg(ap, type)
#define va_end(ap)         __builtin_va_end(ap)

typedef __builtin_va_list va_list;

void kvprintf(const char* fmt, va_list ap) __attribute((format (printf, 1, 0)));

#endif
