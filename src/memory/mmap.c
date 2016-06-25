/* kalloc.c */

#include <ints.h>
#include <klibc.h>
#include <multiboot.h>

#include <system.h>

#define FRAME_SIZE       0x1000
#define FRAME_COUNT      (total_memory / 4)
#define FRAME_TABLE_SIZE (FRAME_COUNT / 32)

#define FREE 0x0
#define USED 0x1

void* kalloc(u32 size);

extern u32 kernel_end;
u32 placement_address = (u32) &kernel_end;

u32 total_memory; // in KB

u32* frame_table;

// not to be used after mmap_init_finalize
void* kalloc(u32 size) {
	void* ret;

	ret = (void*) placement_address;
	placement_address += size;

	return ret;
}

void* kalloc_frame() {
	u32 i;
	u32 j;

	// find a free frame
	for (i = 0; i < FRAME_TABLE_SIZE; i++) {
		if (frame_table[i] == 0xFFFFFFFF) {
			continue;
		}
		for (j = 0; j < 32; j++) {
			// check if frame is free
			if (((frame_table[i] >> j) & 0x1) == FREE) {
				// set frame to used
				frame_table[i] |= (USED << j);
				// return address
				return (void*) ((i * 32 + j) * FRAME_SIZE);
			}
		}
	}

	// NO FREE FRAMES
	return NULL;
}

// assumes contiguous blocks exist
// only to be used early to prevent problems
void* kalloc_frames(u32 num) {
	u32 i;

	u32 address = (u32) kalloc_frame();
	for (i = 1; i < num; i++) {
		mmap_set_used(address + (i * FRAME_SIZE));
	}

	return (void*) address;
}

void mmap_init(u32 size) {
	total_memory = size;
	frame_table = kalloc(FRAME_TABLE_SIZE);
}

void mmap_init_finalize() {
	// TODO: make this more efficient
	//memset(frame_table, 0xFF, (placement_address / FRAME_SIZE) / 8);
	u32 i;

	for (i = 0; i < placement_address; i++) {
		mmap_set_used(i);
	}
}

void mmap_set_free(u32 address) {
	frame_table[(address / FRAME_SIZE) / 32] &= ~ (1 << ((address / FRAME_SIZE) % 32));
}

void mmap_set_used(u32 address) {
	frame_table[(address / FRAME_SIZE) / 32] |= (1 << ((address / FRAME_SIZE) % 32));
}
