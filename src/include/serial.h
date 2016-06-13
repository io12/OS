/* serial.h */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <ints.h>

#include <ioport.h>

#define COM1 0x3F8

void serial_init(u16 com);
int serial_fifo_isempty(u16 com);
void serial_wait();
void serial_putchar(u8 data);
void serial_puts(const char* str);

#endif
