/* serial.h */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <ints.h>

#include "ioport.h"

typedef uint16_t serial_com_t;

// Sets the baud rate for a serial port
inline void serial_set_baud_rate(serial_com_t com, uint16_t div) {
	io_out(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	io_out(SERIAL_DATA_PORT(com), div >> 8);
	io_out(SERIAL_DATA_PORT(com), div & 0x00FF);
}

inline void serial_conf_line(serial_com_t com) {
	io_out(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

#endif
