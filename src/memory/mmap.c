/* mmap.c */

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
u32 kernel_end_address = (u32) &kernel_end;

u32 total_memory; // in KB

u32* frame_table;

// not to be used after mmap_init
void* kalloc(u32 size) {
	void* ret;

	ret = (void*) kernel_end_address;
	kernel_end_address += size;

	return ret;
}

/*void* kalloc_frame() {
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
				// update the kernel end address
				kernel_end_address += FRAME_SIZE;
				// set frame to used
				frame_table[i] |= (USED << j);
				// return address
				return (void*) ((i * 32 + j) * FRAME_SIZE);
			}
		}
	}

	// NO FREE FRAMES
	return NULL;
}*/

// TODO: efficiency
void* kalloc_frames(u32 num) {
	u32 i;
	u32 j;
	u8 found;

	for (i = 0; i < FRAME_COUNT; i++) {
		found = true;
		for (j = 0; j < num; j++) {
			if (mmap_index_check(i + j) == USED) {
				found = false;
				break;
			}
		}
		if (found == true) {
			for (j = 0; j < num; j++) {
				mmap_index_set_used(i + j);
			}
			return (void*) (i * FRAME_SIZE);
		}
	}

	// NOT ENOUGH FRAMES FOUND
	return NULL;
}

// assumes contiguous blocks exist
/*void* kalloc_frames_unsafe(u32 num) {
	u32 i;

	u32 address = (u32) kalloc_frames(1);
	for (i = 1; i < num; i++) {
		mmap_address_set_used(address + (i * FRAME_SIZE));
		kernel_end_address += FRAME_SIZE;
	}

	return (void*) address;
}*/

void kfree_frames(void* ptr, u32 num) {
	u32 address = (u32) ptr;
	u32 i = address;

	while (i < address + (num * FRAME_SIZE)) {
		mmap_address_set_free(address);
		i += FRAME_SIZE;
	}
}

u8 mmap_index_check(u32 n) {
	return (frame_table[n / 32] >> (n % 32)) & 0x1;
}

void mmap_init(u32 size) {
	total_memory = size;
	frame_table = kalloc(FRAME_TABLE_SIZE);
}

void mmap_init_finalize() {
	// TODO: make this more efficient
	//memset(frame_table, 0xFF, (kernel_end_address / FRAME_SIZE) / 8);
	u32 i;

	for (i = 0; i < kernel_end_address; i += FRAME_SIZE) {
		mmap_address_set_used(i);
	}
}

void mmap_address_set_free(u32 address) {
	frame_table[(address / FRAME_SIZE) / 32] &=
		~ (1 << ((address / FRAME_SIZE) % 32));
}

void mmap_address_set_used(u32 address) {
	frame_table[(address / FRAME_SIZE) / 32] |=
		(1 << ((address / FRAME_SIZE) % 32));
}

void mmap_index_set_free(u32 n) {
	frame_table[n / 32] &= ~ (1 << (n % 32));
}

void mmap_index_set_used(u32 n) {
	frame_table[n / 32] |= 1 << (n % 32);
}
