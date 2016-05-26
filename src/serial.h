/* serial.h */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <ints.h>

#include "ioport.h"

#define COM1 0x3F8

typedef uint16_t serial_com_t;
typedef uint8_t	serial_data_t;

void serial_init(serial_com_t com);
int serial_fifo_isempty(serial_com_t com);
void serial_wait();
void serial_putchar(serial_com_t com, serial_data_t data);
void serial_puts(const char* str);

#endif
