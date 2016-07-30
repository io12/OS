#include <interrupt_handler.h>
#include <system.h>

#define SYSCALL_COUNT 1

int _exit(int status);

int (*syscalls[SYSCALL_COUNT])() = {
	_exit,
};

void syscall_handler(InterruptSave* is) {
	if (is->eax >= SYSCALL_COUNT) {
		return;
	}

	syscalls[is->eax](is->ebx, is->ecx, is->edx, is->edi);
}

int _exit(int status) {
	scheduler_exit_current();

	int_wait_forever();

	// NOTREACHED
	return status;
}
