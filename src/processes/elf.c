#include <ints.h>
#include <klibc.h>
#include <kprintf.h>
#include <paging.h>
#include <scheduler.h>
#include <system.h>

#define SHT_NOBITS 8

typedef struct {
	u8  e_ident[16];
	u16 e_type;
	u16 e_machine;
	u32 e_version;
	u32 e_entry;
	u32 e_phoff;
	u32 e_shoff;
	u32 e_flags;
	u16 e_ehsize;
	u16 e_phentsize;
	u16 e_phnum;
	u16 e_shentsize;
	u16 e_shnum;
	u16 e_shstrndx;
} __attribute__((packed))
ELF32_Header;

typedef struct {
	u32 sh_name;
	u32 sh_type;
	u32 sh_flags;
	u32 sh_addr;
	u32 sh_offset;
	u32 sh_size;
	u32 sh_link;
	u32 sh_info;
	u32 sh_addralign;
	u32 sh_entsize;
} ELF32_SectionHeader;

typedef struct {
	u32 p_type;
	u32 p_offset;
	u32 p_vaddr;
	u32 p_paddr;
	u32 p_filesz;
	u32 p_flags;
	u32 p_align;
} __attribute__((packed))
ELF32_ProgramHeader;

void elf_exec(void* file);
u8 elf_verify(ELF32_Header* header);

void elf_exec(void* file) {
	ELF32_Header* header = file;
	ELF32_SectionHeader* sh;
	PageDirectory* page_dir;
	PageTableEntry* stack_page;
	u32 image_low = 0xFFFFFFFF;
	u32 image_high = 0;
	u32 i = 0;
	u32 j = 0;

	if (elf_verify(header) == false) {
		return;
	}
	if (header->e_type != 2) {
		kprintf(PL_ALL, "Unknown ELF type: 0x%X\n", header->e_type);
		return;
	}

	page_dir = paging_make_dir();
	cr3_write((u32) page_dir);

	// iterate through the section headers
	while (i < header->e_shentsize * header->e_shnum) {
		sh = (void*) ((u32) header + (header->e_shoff + i));
		// check if the section should be loaded at an address
		if (sh->sh_addr != 0) {
			// add section header to page directory
			while (j < sh->sh_size) {
				paging_frame_alloc(paging_get_page(
							sh->sh_addr + j,
							page_dir));
				invlpg(sh->sh_addr + j);
				j += 0x1000;
			}
			if (sh->sh_type == SHT_NOBITS) {
				// section is .bss
				memset((void*) sh->sh_addr, 0,
						sh->sh_size);
			}
			else {
				memcpy((void*) sh->sh_addr, (void*) ((u32)
							header +
							sh->sh_offset),
						sh->sh_size);
			}
			if (sh->sh_addr < image_low) {
				image_low = sh->sh_addr;
			}
			if (sh->sh_addr + sh->sh_size > image_high) {
				image_high = sh->sh_addr + sh->sh_size;
			}
		}
		i += header->e_shentsize;
	}

	// allocate a 4KiB stack
	stack_page = paging_get_page(USER_STACK_LOW, page_dir);
	paging_frame_alloc(stack_page);
	stack_page->writable = 1;
	invlpg(USER_STACK_LOW);

	scheduler_new_process(header->e_entry, (u32) page_dir, image_low, image_high);
}

u8 elf_verify(ELF32_Header* header) {
	u16 chk_val;

	if (header == NULL) {
		kprintf(PL_ALL, "Failed to open ELF executable\n");
		return false;
	}
	if (memcmp(header->e_ident, "\177ELF", 4) != 0) {
		kprintf(PL_ALL, "Executed file is not an ELF file\n");
		return false;
	}

	chk_val = header->e_ident[4];
	if (chk_val != 1) {
		kprintf(PL_ALL, "Executable not 32 bit: 0x%X\n", chk_val);
		return false;
	}
	chk_val = header->e_ident[5];
	if (chk_val != 1) {
		kprintf(PL_ALL, "Executable not little endian: 0x%X\n", chk_val);
		return false;
	}
	chk_val = header->e_ident[6];
	if (chk_val != 1) {
		kprintf(PL_ALL, "Unknown ELF version: 0x%X\n", chk_val);
		return false;
	}

	chk_val = header->e_machine;
	if (chk_val != 3) {
		kprintf(PL_ALL, "ELF file machine type not i386: 0x%X\n", chk_val);
		return false;
	}

	return true;
}
