/* ioport.h */

#ifndef _IOPORT_H_
#define _IOPORT_H_

#include <ints.h>

// Sends data to an I/O port
void io_out(u16 port, u8 data);

// Gets data from an I/O port
u8 io_in(u16 port);

#endif
