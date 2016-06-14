/* timer.c */

#include <ints.h>
#include <kprintf.h>

#include <interrupt_handler.h>
#include <ioport.h>

void timer_routine(InterruptSave is);

int timer = 0;

void timer_init() {
	irq_new_routine(0, timer_routine);

	// mask IRQ
	u16 port  = 0x21;
	u8  value = io_in(0x21) | (1 << 0);
	io_out(port, value);

	i32 div = 1193180 / 100;

	io_out(0x43, 0x36);
	io_out(0x40, (div & 0xFF));
	io_out(0x40, (div >> 8));
}

void timer_routine(InterruptSave is) {
	timer++;
	if (timer % 100 == 0) {
		kprintf(PL_SERIAL, "Timer: %d\n", timer / 100);
	}
}
