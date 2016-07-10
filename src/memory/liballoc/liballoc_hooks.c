/* liballoc_hooks.c */

#include <system.h>

#define FRAME_SIZE 0x1000

int liballoc_lock() {
	__asm__ volatile ("cli");

	return 0;
}

int liballoc_unlock() {
	//__asm__ volatile ("sti");

	return 0;
}

void* liballoc_alloc(int num) {
	return kalloc_frames(num);
}

int liballoc_free(void* ptr, int num) {
	u32 address = (u32) ptr;
	u32 i = address;

	while (i < address + (num * FRAME_SIZE)) {
		mmap_address_set_free(address);
		i += FRAME_SIZE;
	}

	return 0;
}
