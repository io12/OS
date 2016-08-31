#include <ints.h>
#include <klibc.h>
#include <kprintf.h>

#include <ext2.h>

#define FS ((Ext2_Filesystem*) fs)

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
#define INDIRECT_COUNT      (FS->block_size / 4)
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
Ext2_Superblock;

typedef struct {
	u32 block_bitmap;
	u32 inode_bitmap;
	u32 inode_table;

	u16 free_blocks_count;
	u16 free_inodes_count;
	u16 used_dirs_count;

	u8  unused[14];
} __attribute__((packed))
Ext2_BlockGroupDescriptor;

typedef struct {
	u32 inode_num;

	u16 rec_len;

	u8  name_len;
	u8  file_type;

	u8  name[255];
} __attribute__((packed))
Ext2_DirEntry;

typedef struct {
	Ext2_Superblock* sb;
	Ext2_BlockGroupDescriptor* bgd_table;
	Ext2_Inode* root_inode;

	u32 base_address;
	u32 block_group_count;
	u32 block_size;
} Ext2_Filesystem;

void* ext2_block_to_ptr(void* fs, u32 block) {
	return (void*) (FS->base_address + (block * FS->block_size));
}

// find a file's inode based on its name
u32 ext2_find_file(void* fs, const Ext2_Inode* inode, const char* fname) {
	u32 i = 0;
	u8* block_ptr = ext2_block_to_ptr(fs, ext2_inode_get_block(fs, inode, 0));
	Ext2_DirEntry* dir_ptr = (Ext2_DirEntry*) block_ptr;

	while (i < inode->size && i < FS->block_size) {
		if (memcmp(fname, dir_ptr->name, dir_ptr->name_len) == 0 &&
				strlen(fname) == dir_ptr->name_len) {
			return dir_ptr->inode_num;
		}
		i += dir_ptr->rec_len;
		block_ptr += dir_ptr->rec_len;
		dir_ptr = (Ext2_DirEntry*) block_ptr;
	}

	// FILE NOT FOUND
	return 0;
}

Ext2_Inode* ext2_get_inode(void* fs, u32 inode_num) {
	u32 block_group = (inode_num - 1) / FS->sb->inodes_per_group;
	Ext2_Inode* inode_table =
		ext2_block_to_ptr(fs, FS->bgd_table[block_group].inode_table);

	return &inode_table[(inode_num - 1) % FS->sb->inodes_per_group];
}

void ext2_init(void* fs, u32 ramdisk_address) {
	FS->base_address = ramdisk_address;
	FS->sb = (void*) (FS->base_address + 1024);

	if (FS->sb->signature != 0xEF53) {
		kprintf(PL_ALL, "Signature on ext2 ramdisk 0x%X not valid. "
				"Should be 0xEF53.\n", FS->sb->signature);
	}

	// get the block group count by dividing the inode count by the amount of
	// inodes per group and rounding up
	FS->block_group_count = FS->sb->inode_count / FS->sb->inodes_per_group;
	if (FS->sb->inode_count % FS->sb->inodes_per_group != 0) {
		FS->block_group_count++;
	}

	FS->block_size = 1024 << FS->sb->log2_block_size;

	if (FS->block_size == 1024) {
		FS->bgd_table = (void*) (ramdisk_address + 2048);
	}
	else {
		FS->bgd_table = (void*) (ramdisk_address + FS->block_size);
	}

	FS->root_inode = ext2_get_inode(fs, 2);
}

u32 ext2_inode_get_block(void* fs, const Ext2_Inode* inode, u32 block) {
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
		ptr1 = ext2_block_to_ptr(fs, inode->block[12]);
		return ptr1[n1];
	}
	// doubly indirect
	if (block <= DOUBLY_INDIRECT_MAX) {
		ptr1 = ext2_block_to_ptr(fs, inode->block[13]);
		ptr2 = ext2_block_to_ptr(fs, ptr1[n2]);
		return ptr2[n3];
	}
	// triply indirect
	if (block <= TRIPLY_INDIRECT_MAX) {
		ptr1 = ext2_block_to_ptr(fs, inode->block[14]);
		ptr2 = ext2_block_to_ptr(fs, ptr1[n4]);
		ptr3 = ext2_block_to_ptr(fs, ptr2[n5]);
		return ptr3[n6];
	}

	// BLOCK NUMBER TOO HIGH
	return 0;
}

u32 ext2_path_to_inode_num(void* fs, u32 inode_num, char* fpath) {
	Ext2_Inode* inode = ext2_get_inode(fs, inode_num);
	char* slash_ptr;
	u32 i = 0;
	u32 depth = 0;

	// handle absolute paths
	if (*fpath == '/') {
		fpath++;
		inode_num = 2;
		inode = FS->root_inode;
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

		inode_num = ext2_find_file(fs, inode, fpath);
		if (inode_num == 0) {
			// FILE NOT FOUND
			return 0;
		}

		inode = ext2_get_inode(fs, inode_num);
		fpath = slash_ptr + 1;
	}

	return inode_num;
}

ssize_t ext2_read(void* fs, u32 inode_num, void* buf, size_t count,
		u32 pos) {
	Ext2_Inode* inode = ext2_get_inode(fs, inode_num);
	u8* block_ptr;
	u32 block_num;
	u32 n_into_block;
	u32 count_in_block;
	ssize_t ret = 0;

	if (pos > inode->size) {
		return 0;
	}
	if (pos + count > inode->size) {
		count = inode->size - pos;
	}

	block_num    = ext2_inode_get_block(fs, inode, pos / FS->block_size);
	block_ptr    = ext2_block_to_ptr(fs, block_num);
	n_into_block = pos % FS->block_size;
	block_ptr   += n_into_block;

	for (;;) {
		count_in_block = FS->block_size - n_into_block;
		if (count_in_block >= count) {
			memcpy(buf + ret, block_ptr, count);
			ret += count;
			break;
		}
		else {
			memcpy(buf + ret, block_ptr, count_in_block);
			ret         += count_in_block;
			pos         += count_in_block;
			block_num    = ext2_inode_get_block(fs, inode,
					pos / FS->block_size);
			block_ptr    = ext2_block_to_ptr(fs, block_num);
			count       -= count_in_block;
			n_into_block = 0;
		}
	}

	return ret;
}
