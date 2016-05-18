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

// Serial
// bases
#define SERIAL_COM1_BASE		0x3F1
// ports
#define SERIAL_DATA_PORT(base)		(base)
#define SERIAL_FIFO_COMMAND_PORT(base)	(base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)	(base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)	(base + 4)
#define SERIAL_LINE_STATUS_PORT(base)	(base + 5)
// commands
#define SERIAL_LINE_ENABLE_DLAB		0x80

typedef uint16_t ioport_t;
typedef uint8_t iodata_t;

// Sends out data to a specified I/O port
void io_out(ioport_t port, iodata_t data);

#endif
