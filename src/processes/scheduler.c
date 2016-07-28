#include <interrupt_handler.h>
#include <klibc.h>
#include <kprintf.h>
#include <liballoc.h>

#include <system.h>

typedef struct process {
	InterruptSave is;
	u32 cr3;
	u32 entry;
	u8  started;
	struct process* next;
} Process;

void scheduler_callback(InterruptSave* is);

Process* process_queue;
Process* process_current;

void scheduler_init() {
	// A divisor needs to be sent to the PIT that divides it's internal
	// frequency to get a desired frequency.
	// This divisor results in the frequency being 100.
	u16 div = 1193182 / 100;

	irq_new_routine(0, scheduler_callback);

	// initialize PIT
	out(0x43, 0x36);
	out(0x40, (div & 0xFF));
	out(0x40, (div >> 8));

	// intialize process queue
	process_queue   = NULL;
	process_current = NULL;
}

void scheduler_new_process(u32 entry, u32 cr3) {
	Process* process = process_current;

	if (process == NULL) {
		process = malloc(sizeof(Process));
		process_queue   = process;
		process_current = process_queue;
	}
	else {
		while (process->next != NULL) {
			process = process->next;
		}
		process->next = malloc(sizeof(Process));
		process = process->next;
	}

	process->cr3     = cr3;
	process->entry   = entry;
	process->started = false;
	process->next    = NULL;
}

void scheduler_callback(InterruptSave* is) {
	if (process_current == NULL) {
		kprintf(PL_SERIAL, "Process switcher called, but no process is "
				"loaded\n");
		return;
	}

	// save registers
	memcpy(&process_current->is, is, sizeof(InterruptSave));

	// switch to the next process, or recycle if necessary
	if (process_current->next == NULL) {
		process_current = process_queue;
	}
	else {
		process_current = process_current->next;
	}

	// change the registers to the new process
	memcpy(is, &process_current->is, sizeof(InterruptSave));
	cr3_write(process_current->cr3);

	// jump to entry point of the executable if it hasn't been started yet
	if (process_current->started == false) {
		process_current->started = true;
		is->eip = process_current->entry;
		// enter user mode if needed
		if (is->cs != 0x1B) {
			is->gs = 0x23;
			is->fs = 0x23;
			is->es = 0x23;
			is->ds = 0x23;

			is->cs = 0x1B;
			is->ss = 0x23;
		}
	}
}
