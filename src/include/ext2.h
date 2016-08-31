#ifndef _EXT2_H_
#define _EXT2_H_

#include <ints.h>

typedef struct {
	u16 type;
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
Ext2_Inode;

void* ext2_block_to_ptr(void* fs, u32 block);
u32 ext2_find_file(void* fs, const Ext2_Inode* inode, const char* fname);
Ext2_Inode* ext2_get_inode(void* fs, u32 inode_num);
void ext2_init(void* fs, u32 ramdisk_address);
u32 ext2_inode_get_block(void* fs, const Ext2_Inode* inode, u32 block);
u32 ext2_path_to_inode_num(void* fs, u32 inode_num, char* fpath);

#endif
