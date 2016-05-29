/* ioport.h */

#ifndef _IOPORT_H_
#define _IOPORT_H_

#include <ints.h>

typedef uint16_t ioport_t;
typedef uint8_t iodata_t;

// Sends out data to a specified I/O port
void io_out(ioport_t port, iodata_t data);

iodata_t io_in(ioport_t port);

#endif
