/* timer.c */

#include <kprintf.h>

#include <interrupt_handler.h>
#include <ioport.h>

int timer_ticks = 0;

void timer_handler(InterruptSave is) {
	timer_ticks++;
	if (timer_ticks % 18 == 0) {
		kprintf("TIMER: %d\n", timer_ticks / 18);
	}
}

void timer_init() {
	irq_new_routine(0, timer_handler);
}

void timer_phase(int hz) {
	int div = 1193180 / hz;

	io_out(0x43, 0x36);
	io_out(0x40, div & 0xFF);
	io_out(0x40, div >> 8);
}
