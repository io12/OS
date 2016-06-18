/* paging.c */

#include <ints.h>

#define PAGE_SIZE 0x1000

u32 page_directory[1024] __attribute__((aligned(4096)));

void paging_init() {
	u32 i;

	for (i = 0; i < 1024; i++) {
		page_directory[i] = 0x00000002;
	}
}
