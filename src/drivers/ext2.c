/* ext2.c */

#include <ints.h>
#include <kprintf.h>

#include <system.h>

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
	u16 type_and_permissions;
	u16 uid;

	u32 size;
	u32 last_access_time;
	u32 creation_time;
	u32 last_modification_time;
	u32 deletion_time;

	u16 gid;
	u16 hard_link_count;

	u32 sectors_used_count;
	u32 flags;
	u32 os_specific_val_1;
	u32 block[15];
	u32 generation_num;
	u32 file_acl;
	u32 dir_acl;
	u32 fragment_block;

	u8  os_specific_val_2[12];
} __attribute__((packed))
Ext2Inode;

typedef struct {
	u32 inode_num;

	u16 rec_len;

	u8  name_len;
	u8  file_type;

	u8  name[];
} __attribute__((packed))
Ext2DirectoryEntry;

void* ext2_block_to_ptr(u32 block);
Ext2Inode* ext2_get_inode(u32 inode_num);

Ext2Superblock* ext2_sb;
Ext2BlockGroupDescriptor* ext2_bgd_table;

u32 ext2_base_address;
u32 ext2_block_group_count;
u32 ext2_block_size;

void* ext2_block_to_ptr(u32 block) {
	return (void*) (ext2_base_address + (block * ext2_block_size));
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
		kprintf(PL_FRAMEBUFFER, "Signature on ext2 ramdisk 0x%X not valid. "
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

	// TODO:
	Ext2Inode* root = ext2_get_inode(2);
	Ext2DirectoryEntry* root_dir = ext2_block_to_ptr(root->block[0]);
	kprintf(PL_FRAMEBUFFER, "%s\n", root_dir->name);
	root_dir = ((void*) root_dir) + root_dir->rec_len;
	kprintf(PL_FRAMEBUFFER, "%s\n", root_dir->name);
	root_dir = ((void*) root_dir) + root_dir->rec_len;
	kprintf(PL_FRAMEBUFFER, "%s\n", root_dir->name);
	root_dir = ((void*) root_dir) + root_dir->rec_len;
	kprintf(PL_FRAMEBUFFER, "%s\n", root_dir->name);
	kprintf(PL_FRAMEBUFFER, "%s\n", ext2_block_to_ptr(ext2_get_inode(root_dir->inode_num)->block[0]));
}
