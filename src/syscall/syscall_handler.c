#include <interrupt_handler.h>
#include <ints.h>
#include <kprintf.h>
#include <scheduler.h>
#include <system.h>

#define SYSCALL_COUNT 2

#define IS_IN_STACK(ptr) \
	(!(ptr > USER_STACK_HIGH || ptr < USER_STACK_LOW))

#define IS_IN_IMAGE(ptr) \
	(!(ptr > process_current->image_high || ptr < process_current->image_low))

int _exit(int status);
int open(const char* pathname, int flags, int mode);

int (*syscalls[SYSCALL_COUNT])() = {
	_exit,
	open,
};

void syscall_handler(InterruptSave* is) {
	if (is->eax >= SYSCALL_COUNT) {
		return;
	}

	syscalls[is->eax](is->ebx, is->ecx, is->edx, is->edi);
}

void syscall_validate_ptr(const void* ptr) {
	if (!(IS_IN_STACK((u32) ptr) || IS_IN_IMAGE((u32) ptr))) {
		// invalid pointer, cause a segfault
		kprintf(PL_ALL, "segmentation fault :(\n");
		_exit(1);
	}
}

int _exit(int status) {
	scheduler_exit_current();

	int_wait_forever();

	// NOTREACHED
	return status;
}

int open(const char* pathname, int flags, int mode) {
	syscall_validate_ptr(pathname);
}
