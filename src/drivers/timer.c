/* timer.c */

#include <ints.h>
#include <kprintf.h>

#include <interrupt_handler.h>
#include <ioport.h>

void timer_callback();

int timer = 0;

void timer_init() {
	// A divisor needs to be sent to the PIT that divides it's internal
	// frequency to get a desired frequency.
	// This divisor results in the frequency being 100.
	u16 div = 1193182 / 100;

	irq_new_routine(0, timer_callback);

	io_out(0x43, 0x36);
	io_out(0x40, (div & 0xFF));
	io_out(0x40, (div >> 8));
}

void timer_callback() {
	timer++;
	if (timer % 100 == 0) {
		// runs every second
		kprintf(PL_FRAMEBUFFER, "%u\n", timer / 100);
	}
}
