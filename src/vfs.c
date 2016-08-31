#include <ints.h>
#include <klibc.h>
#include <liballoc.h>

typedef struct filesystem_node {
	char* name;
	void* fs_data;
	u32 mask;
	u32 uid;
	u32 gid;
	u32 flags;
	u32 inode;
	u32 length;

	u32 atime;
	u32 mtime;
	u32 ctime;

	struct filesystem_node* symlink;
	u32 hardlink_count;
} FilesystemNode;

VFS_MountpointList* vfs_mountpoints = NULL;
u32 ramdisk_count = 0;

void vfs_mount(const char* path, int fs_type, void* fs_data) {
	VFS_MountpointList* mountpoint = vfs_mountpoints;

	// allocate a new entry
	if (mountpoint == NULL) {
		mountpoint = malloc(sizeof(VFS_MountpointList));
		vfs_mountpoints = mountpoint;
	}
	else {
		while (mountpoint->next != NULL) {
			mountpoint = mountpoint->next;
		}
		mountpoint->next = malloc(sizeof(VFS_MountpointList));
		mountpoint = mountpoint->next;
	}

	mountpoint->path    = strdup(path);
	mountpoint->fs_type = fs_type;
	mountpoint->fs_data = fs_data;
	mountpoint->next    = NULL;
}

void vfs_ramdisk_attach(void* ptr) {
	char* path[64];

	sprintf(path, "/dev/ram%u", ramdisk_count);
	ramdisk_count++;

	vfs_mount(path, FS_TYPE_EXT2_DEV_RAMDISK, ptr);
}

void vfs_ramdisk_mount(void)

VFS_File* vfs_open() {
}
