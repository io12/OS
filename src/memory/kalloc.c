/* kalloc.c */

#include <ints.h>
#include <klibc.h>

#include <system.h>

#define FRAME_SIZE 0x1000

#define FREE 0x00
#define USED 0x01

extern kernel_end;
u32 free_memory = (u32) &kernel_end;

u8 *frame_table;
u32 frame_count;

void* kalloc() {
	u32 i;

	for (i = 0; i < frame_count; i++) {
		if (frame_table[i] == FREE) {
			frame_table[i] = USED;
			return kernel_end + (FRAME_SIZE * i);
		}
	}
}

void kfree(void* ptr) {
	frame_table[(ptr - kernel_end) / FRAME_SIZE] = FREE;
}

void memory_init(u32 mem_size) {
	// set the frame pointer after the stack
	frame_table  = kernel_end;
	// set the frame count to the amount of free memory
	frame_count  = (mem_size - kernel_end) / FRAME_SIZE;
	// subtract the value of the frames taken by the frame table
	frame_count -= frame_count / FRAME_SIZE;
	// push the pointer after the frame table
	kernel_end  += frame_count;

	// clear all page table entries
	memset(frame_table, FREE, frame_count);
}
