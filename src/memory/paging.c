/* paging.c */

#include <ints.h>
#include <klibc.h>

#include <system.h>

#define PAGE_SIZE 0x1000

u32 cr0_read();
void cr0_write(u32 cr0);
u32 cr3_read();
void cr3_write(u32 cr3);

typedef volatile struct {
	u32 present       : 1;
	u32 writable      : 1;
	u32 user          : 1;
	u32 write_through : 1;
	u32 cache_disable : 1;
	u32 accessed      : 1;
	u32 dirty         : 1;
	u32 attribute     : 1;
	u32 global        : 1;
	u32 available     : 3;
	u32 address       : 20;
} __attribute__((packed))
PageTableEntry;

typedef struct {
	PageTableEntry entries[1024];
} PageTable;

typedef volatile struct {
	u32 present       : 1;
	u32 writable      : 1;
	u32 user          : 1;
	u32 write_through : 1;
	u32 cache_disable : 1;
	u32 accessed      : 1;
	u32 reserved      : 1;
	u32 page_size     : 1;
	u32 global        : 1;
	u32 available     : 3;
	u32 address       : 20;
} __attribute__((packed))
PageDirectoryEntry;

typedef struct {
	PageDirectoryEntry entries[1024];
} PageDirectory;

/*u8 paging_alloc(PageTableEntry* page) {
	u32 address = (u32) kalloc_frame();
	if (address == 0) {
		return false;
	}

	page->frame   = address >> 12;
	page->present = 1;

	return true;
}*/

/*void paging_free(Page* page) {
	u32 address = page->frame << 12;
	if (address != 0) {
		mmap_set_free(address);
	}

	page->present = 0;
}*/

void paging_init() {
	u32 i;
	u32 cr0;

	// identity map a page table
	PageTable* page_table = kalloc_frame();
	memset(page_table, 0, sizeof(PageTable));
	for (i = 0; i < 1024; i++) {
		page_table->entries[i].present = 1;
		page_table->entries[i].address = (i * PAGE_SIZE) >> 12;
	}

	// create a page directory
	PageDirectory* page_directory = kalloc_frame();
	memset(page_directory, 0, sizeof(PageDirectory));
	page_directory->entries[0].present  = 1;
	page_directory->entries[0].writable = 1;
	page_directory->entries[0].address  = (u32) page_table >> 12;

	// enable the page directory
	cr3_write((u32) page_directory);
	cr0 = cr0_read();
	cr0 |= 0x80000000; // set the 32nd bit
	cr0_write(cr0);
}
