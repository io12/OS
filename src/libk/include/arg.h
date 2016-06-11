/* arg.h */

// Similar to stdarg.h

#ifndef _ARG_H_
#define _ARG_H_

typedef char* va_list;

#define va_start(ap, last)	(ap = (va_list) &last + sizeof(last))
#define va_arg(ap, type)	(*(type*)(ap += sizeof(type), ap - sizeof(type)))
#define va_end(ap)		(ap = (va_list)0)

#endif
