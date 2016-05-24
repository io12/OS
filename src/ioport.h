/* ioport.h */

#ifndef _IOPORT_H_
#define _IOPORT_H_

#include <ints.h>

// All the macros below are I/O commands and ports

// Framebuffer
// ports
#define FB_COMMAND_PORT			0x3D4
#define FB_DATA_PORT			0x3D5
// commands
#define FB_HIGH_BYTE_COMMAND		14
#define FB_LOW_BYTE_COMMAND		15

typedef uint16_t ioport_t;
typedef uint8_t iodata_t;

// Sends out data to a specified I/O port
void io_out(ioport_t port, iodata_t data);

iodata_t io_in(ioport_t port);

#endif
