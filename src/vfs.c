/* vfs.c */

#include <ext2.h>
#include <ints.h>
#include <klibc.h>

ssize_t read(u32 inode_num, void* buf, size_t count, u32 pos) {
	Ext2Inode* inode = ext2_get_inode(inode_num);
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

	block_num    = ext2_inode_get_block(inode, pos / ext2_block_size);
	block_ptr    = ext2_block_to_ptr(block_num);
	n_into_block = pos % ext2_block_size;
	block_ptr   += n_into_block;

	// TODO: loop this
	count_in_block = ext2_block_size - n_into_block;
	if (count_in_block < count) {
		memcpy(buf, block_ptr, count_in_block);
		ret   += count_in_block;
		count -= count_in_block;
	}
	else {
		memcpy(buf, block_ptr, count);
		ret += count;
	}

	return ret;
}
