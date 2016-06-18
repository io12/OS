/* serial.c */

#include <klibc.h>

#include <system.h>

void serial_init(u16 com) {
	// Disable interrupts
	io_out(com + 1, 0x00);
	// Enable DLAB
	io_out(com + 3, 0x80);
	// Set baud rate divisor to 3
	io_out(com + 0, 0x03);
	io_out(com + 1, 0x00);
	// Set no parity
	io_out(com + 3, 0x03);
	// Enable FIFO, clear, 14-bytes
	io_out(com + 2, 0xC7);
	// IRQs enabled, RTS/DSR set
	io_out(com + 4, 0x0B);
}

// Checks if the fifo is empty
int serial_fifo_isempty(u16 com) {
	return io_in(com + 5) & 0x20;
}

// Wait to do a serial action
void serial_wait() {
	while (serial_fifo_isempty(COM1) == 0) {}
}

// Write to a serial port
void serial_putchar(u8 data) {
	// Wait till the buffer is flushed
	serial_wait();
	// Write the data
	io_out(COM1, data);
}

void serial_puts(const char* str) {
	size_t i;
	size_t length = strlen(str);

	for (i = 0; i < length; i++) {
		serial_putchar(str[i]);
	}
}
