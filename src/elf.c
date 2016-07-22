/* elf.c */

#include <ints.h>
#include <klibc.h>
#include <kprintf.h>

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
Elf32Header;

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
} Elf32_Shdr;

typedef struct {
	u32 p_type;
	u32 p_offset;
	u32 p_vaddr;
	u32 p_paddr;
	u32 p_filesz;
	u32 p_flags;
	u32 p_align;
} __attribute__((packed))
Elf32ProgramHeader;

void* elf_load(void* file) {
	Elf32Header* elf_header = file;
	if (elf_verify(elf_header) == false) {
		return NULL;
	}

	if (elf_header->e_type == 1) {
		// TODO:
		kprintf(PL_ALL, "Relocatable ELF files not implemented yet\n");
		return NULL;
	}
	if (elf_header->e_type == 2) {
		return (void*) elf_header->e_entry;
	}

	kprintf(PL_ALL, "Unknown ELF type: 0x%X\n", elf_header->e_type);
	return NULL;
}

u8 elf_verify(Elf32Header* elf_header) {
	u16 chk_val;

	if (elf_header == NULL) {
		kprintf(PL_ALL, "Failed to open ELF executable\n");
		return false;
	}
	if (memcmp(elf_header->e_ident, "\177ELF", 4) != 0) {
		kprintf(PL_ALL, "Executed file is not an ELF file\n");
		return false;
	}

	chk_val = elf_header->e_ident[4];
	if (chk_val != 1) {
		kprintf(PL_ALL, "Executable not 32 bit: 0x%X\n", chk_val);
		return false;
	}
	chk_val = elf_header->e_ident[5];
	if (chk_val != 1) {
		kprintf(PL_ALL, "Executable not little endian: 0x%X\n", chk_val);
		return false;
	}
	chk_val = elf_header->e_ident[6];
	if (chk_val != 1) {
		kprintf(PL_ALL, "Unknown ELF version: 0x%X\n", chk_val);
		return false;
	}

	chk_val = elf_header->e_machine;
	if (chk_val != 3) {
		kprintf(PL_ALL, "ELF file machine type not i386: 0x%X\n", chk_val);
		return false;
	}

	return true;
}
