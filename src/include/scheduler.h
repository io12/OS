#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <interrupt_handler.h>

typedef struct process {
	InterruptSave is;
	u32 cr3;
	u32 entry;
	u8  started;
	u32 image_low;
	u32 image_high;
	struct process* next;
} Process;

void scheduler_init();
void scheduler_new_process(u32 entry, u32 cr3, u32 image_low, u32 image_high);
void scheduler_exit_current();

extern Process* process_current;

#endif
