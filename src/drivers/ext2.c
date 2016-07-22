/* ext2.c */

#include <ints.h>
#include <klibc.h>
#include <kprintf.h>

#include <ext2.h>

#define TYPE_FIFO  0x1000
#define TYPE_CHAR  0x2000
#define TYPE_DIR   0x4000
#define TYPE_BLOCK 0x6000
#define TYPE_FILE  0x8000
#define TYPE_SYM   0xA000
#define TYPE_SOCK  0xC000

#define TYPE_MASK  0xF000

// TODO: optimize
// amount of references in a block pointer block
#define INDIRECT_COUNT      (ext2_block_size / 4)
// max blocks possible with different addressing methods
#define DIRECT_MAX          11
#define SINGLY_INDIRECT_MAX (DIRECT_MAX + INDIRECT_COUNT)
#define DOUBLY_INDIRECT_MAX (SINGLY_INDIRECT_MAX + INDIRECT_COUNT * INDIRECT_COUNT)
#define TRIPLY_INDIRECT_MAX (DOUBLY_INDIRECT_MAX + INDIRECT_COUNT * INDIRECT_COUNT \
		* INDIRECT_COUNT)

typedef struct {
	u32 inode_count;
	u32 block_count;
	u32 su_block_count;
	u32 free_block_count;
	u32 free_inode_count;
	u32 first_data_block;
	u32 log2_block_size;
	u32 log2_fragment_size;
	u32 blocks_per_group;
	u32 fragments_per_group;
	u32 inodes_per_group;
	u32 last_mount_time;
	u32 last_write_time;

	u16 mounted_since_fsck;
	u16 mounts_before_fsck;
	u16 signature;
	u16 state;
	u16 do_on_error;
	u16 version_minor;

	u32 last_fsck_time;
	u32 time_between_fscks;
	u32 os_id;
	u32 version_major;

	u16 uid_reserved_blocks;
	u16 gid_reserved_blocks;
} __attribute__((packed))
Ext2Superblock;

typedef struct {
	u32 block_bitmap;
	u32 inode_bitmap;
	u32 inode_table;

	u16 free_blocks_count;
	u16 free_inodes_count;
	u16 used_dirs_count;

	u8  unused[14];
} __attribute__((packed))
Ext2BlockGroupDescriptor;

typedef struct {
	u32 inode_num;

	u16 rec_len;

	u8  name_len;
	u8  file_type;

	u8  name[255];
} __attribute__((packed))
Ext2DirEntry;

Ext2Superblock* ext2_sb;
Ext2BlockGroupDescriptor* ext2_bgd_table;
Ext2Inode* ext2_root_inode;

u32 ext2_base_address;
u32 ext2_block_group_count;
u32 ext2_block_size;

void* ext2_block_to_ptr(u32 block) {
	return (void*) (ext2_base_address + (block * ext2_block_size));
}

// find a file's inode based on its name
u32 ext2_find_file(const Ext2Inode* inode, const char* fname) {
	u32 i = 0;
	u8* block_ptr = ext2_block_to_ptr(ext2_inode_get_block(inode, 0));
	Ext2DirEntry* dir_ptr = (Ext2DirEntry*) block_ptr;

	while (i < inode->size && i < ext2_block_size) {
		if (memcmp(fname, dir_ptr->name, dir_ptr->name_len) == 0 &&
				strlen(fname) == dir_ptr->name_len) {
			return dir_ptr->inode_num;
		}
		i += dir_ptr->rec_len;
		block_ptr += dir_ptr->rec_len;
		dir_ptr = (Ext2DirEntry*) block_ptr;
	}

	// FILE NOT FOUND
	return 0;
}

Ext2Inode* ext2_get_inode(u32 inode_num) {
	u32 block_group = (inode_num - 1) / ext2_sb->inodes_per_group;
	Ext2Inode* inode_table =
		ext2_block_to_ptr(ext2_bgd_table[block_group].inode_table);

	return &inode_table[(inode_num - 1) % ext2_sb->inodes_per_group];
}

void ext2_init(u32 ramdisk_address) {
	ext2_base_address = ramdisk_address;
	ext2_sb = (void*) (ext2_base_address + 1024);

	if (ext2_sb->signature != 0xEF53) {
		kprintf(PL_ALL, "Signature on ext2 ramdisk 0x%X not valid. "
				"Should be 0xEF53.\n", ext2_sb->signature);
	}

	// get the block group count by dividing the inode count by the amount of
	// inodes per group and rounding up
	ext2_block_group_count = ext2_sb->inode_count / ext2_sb->inodes_per_group;
	if (ext2_sb->inode_count % ext2_sb->inodes_per_group != 0) {
		ext2_block_group_count++;
	}

	ext2_block_size = 1024 << ext2_sb->log2_block_size;

	if (ext2_block_size == 1024) {
		ext2_bgd_table = (void*) (ramdisk_address + 2048);
	}
	else {
		ext2_bgd_table = (void*) (ramdisk_address + ext2_block_size);
	}

	ext2_root_inode = ext2_get_inode(2);
}

u32 ext2_inode_get_block(const Ext2Inode* inode, u32 block) {
	// depth pointers
	u32* ptr1;
	u32* ptr2;
	u32* ptr3;
	// block indexes
	u32 n1 = block - 12;
	u32 n2 = (block - SINGLY_INDIRECT_MAX) / INDIRECT_COUNT;
	u32 n3 = (block - SINGLY_INDIRECT_MAX) % INDIRECT_COUNT;
	u32 n4 = (block - DOUBLY_INDIRECT_MAX) / (INDIRECT_COUNT * INDIRECT_COUNT);
	u32 n5 = (block - DOUBLY_INDIRECT_MAX) % (INDIRECT_COUNT * INDIRECT_COUNT) /
		INDIRECT_COUNT;
	u32 n6 = (block - DOUBLY_INDIRECT_MAX) % (INDIRECT_COUNT * INDIRECT_COUNT) %
		INDIRECT_COUNT;

	// direct
	if (block <= DIRECT_MAX) {
		return inode->block[block];
	}
	// singly indirect
	if (block <= SINGLY_INDIRECT_MAX) {
		ptr1 = ext2_block_to_ptr(inode->block[12]);
		return ptr1[n1];
	}
	// doubly indirect
	if (block <= DOUBLY_INDIRECT_MAX) {
		ptr1 = ext2_block_to_ptr(inode->block[13]);
		ptr2 = ext2_block_to_ptr(ptr1[n2]);
		return ptr2[n3];
	}
	// triply indirect
	if (block <= TRIPLY_INDIRECT_MAX) {
		ptr1 = ext2_block_to_ptr(inode->block[14]);
		ptr2 = ext2_block_to_ptr(ptr1[n4]);
		ptr3 = ext2_block_to_ptr(ptr2[n5]);
		return ptr3[n6];
	}

	// BLOCK NUMBER TOO HIGH
	return 0;
}

u32 ext2_path_to_inode_num(u32 inode_num, char* fpath) {
	Ext2Inode* inode = ext2_get_inode(inode_num);
	char* slash_ptr;
	u32 i = 0;
	u32 depth = 0;

	// handle absolute paths
	if (*fpath == '/') {
		fpath++;
		inode_num = 2;
		inode = ext2_root_inode;
	}

	// calculate file depth
	slash_ptr = fpath;
	while (*slash_ptr != '\0') {
		if (*slash_ptr == '/') {
			depth++;
		}
		slash_ptr++;
	}

	// iterate through the path
	for (i = 0; i <= depth; i++) {
		slash_ptr = strchr(fpath, '/');
		if (slash_ptr != NULL) {
			*slash_ptr = '\0';
		}

		inode_num = ext2_find_file(inode, fpath);
		if (inode_num == 0) {
			// FILE NOT FOUND
			return 0;
		}

		inode = ext2_get_inode(inode_num);
		fpath = slash_ptr + 1;
	}

	return inode_num;
}
