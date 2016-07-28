/* paging.h */

#ifndef _PAGING_H_
#define _PAGING_H_

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

void paging_init();
PageTableEntry* paging_get_page(u32 address, PageDirectory* page_dir);
void paging_frame_alloc(PageTableEntry* page);
void paging_frame_free(PageTableEntry* page);
PageDirectory* paging_make_dir();
void paging_free_dir();

#endif
