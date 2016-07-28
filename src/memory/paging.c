#include <ints.h>
#include <klibc.h>
#include <system.h>

#include <paging.h>

#define PAGE_SIZE 0x1000

#define SHIFT(address) ((address) >> 12)
#define UNSHIFT(address) ((address) << 12)

PageDirectory* kernel_page_directory;
PageDirectory* user_page_directory;

void paging_init() {
	u32 i;
	u32 cr0;
	PageTable* page_table;

	// identity map a page table
	page_table = kalloc_frames(1);
	memset(page_table, 0, sizeof(PageTable));
	for (i = 0; i < 1024; i++) {
		page_table->entries[i].present  = 1;
		page_table->entries[i].writable = 1;
		page_table->entries[i].address  = SHIFT(i * PAGE_SIZE);
	}

	// create a page directory
	kernel_page_directory = kalloc_frames(1);
	memset(kernel_page_directory, 0, sizeof(PageDirectory));
	kernel_page_directory->entries[0].present  = 1;
	kernel_page_directory->entries[0].writable = 1;
	kernel_page_directory->entries[0].address  = SHIFT((u32) page_table);

	// enable the page directory
	cr3_write((u32) kernel_page_directory);
	cr0 = cr0_read();
	// set the 32nd bit of cr0
	cr0 |= 0x80000000;
	cr0_write(cr0);
}

PageTableEntry* paging_get_page(u32 address, PageDirectory* page_dir) {
	u32 n;
	PageTable* page_table;

	address /= PAGE_SIZE;
	n = address / 1024;

	if (page_dir->entries[n].present == 0) {
		page_table = kalloc_frames(1);
		memset(page_table, 0, sizeof(PageTable));

		page_dir->entries[n].address  = SHIFT((u32) page_table);
		page_dir->entries[n].present  = 1;
		page_dir->entries[n].writable = 1;
		page_dir->entries[n].user     = 1;
	}
	else {
		page_table = (void*) UNSHIFT(page_dir->entries[n].address);
	}

	return &page_table->entries[address % 1024];
}

void paging_frame_alloc(PageTableEntry* page) {
	void* ptr = kalloc_frames(1);

	if (page->address != 0) {
		paging_frame_free(page);
	}
	page->address  = SHIFT((u32) ptr);
	page->present  = 1;
	page->user     = 1;
}

void paging_frame_free(PageTableEntry* page) {
	kfree_frames((void*) UNSHIFT(page->address), 1);
	memset((void*) page, 0, sizeof(PageTableEntry));
}

PageDirectory* paging_make_dir() {
	PageDirectory* ret = kalloc_frames(1);

	memcpy(ret, kernel_page_directory, sizeof(PageDirectory));

	return ret;
}

void paging_free_dir(PageDirectory* page_dir) {
	kfree_frames(page_dir, 1);
}
