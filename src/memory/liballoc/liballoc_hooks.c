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
	kfree_frames(ptr, num);

	return 0;
}
