/* scheduler.c */

#include <interrupt_handler.h>
#include <klibc.h>
#include <kprintf.h>

#include <system.h>

typedef struct process {
	InterruptSave is;
	u32 cr3;
	struct process* next;
} Process;

Process* process_queue;
Process* process_current;

void process_switch_callback(InterruptSave is);

void scheduler_init() {
	// A divisor needs to be sent to the PIT that divides it's internal
	// frequency to get a desired frequency.
	// This divisor results in the frequency being 100.
	u16 div = 1193182 / 100;

	irq_new_routine(0, process_switch_callback);

	// initialize PIT
	io_out(0x43, 0x36);
	io_out(0x40, (div & 0xFF));
	io_out(0x40, (div >> 8));

	// TODO: fix this with 245+ processes
	// intialize process queue
	process_queue = kalloc_frames(5);
	process_current = process_queue;
	process_current->next = NULL;
}

void process_switch_callback(InterruptSave is) {
	// save registers
	memcpy(&process_current->is, &is, sizeof(InterruptSave));

	// switch to the next process, or recycle if necessary
	if (process_current->next == NULL) {
		process_current = process_queue;
	}
	else {
		process_current = process_current->next;
	}

	// change the registers to the new process
	memcpy(&is, &process_current->is, sizeof(InterruptSave));
	//cr3_write(process_current->cr3);
}
