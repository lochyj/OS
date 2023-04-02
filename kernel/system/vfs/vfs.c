#include "kernel/system/vfs/vfs.h"

struct fs_node* fs_root = 0;	/* The root filesystem */

/* Read from an inode in the filesystem */
uint32_t read_fs(struct fs_node* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
	if (node->read != 0)
		return node->read(node, offset, size, buffer);
	else
		return 0;
}

/* Write to an inode in the filesystem */
uint32_t write_fs(struct fs_node* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
	if (node->write != 0)
		return node->write(node, offset, size, buffer);
	else
		return 0;
}

/* Opens an inode in the filesystem */
void open_fs(struct fs_node* node, uint8_t read, uint8_t write) {
	if (node->open != 0)
		return node->open(node);
}

/* Closes an inode in the filesystem */
void close_fs(struct fs_node* node) {
	if (node->close != 0)
		return node->close(node);
}

/* Reads from a directory in the filesystem */
struct dirent* readdir_fs(struct fs_node* node, uint32_t index) {
	if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != 0)
		return node->readdir(node, index);
	else
		return 0;
}

/* Finds a child in a directory in the filesystem */
struct fs_node* finddir_fs(struct fs_node* node, char* name) {
	if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != 0)
		return node->finddir(node, name);
	else
		return 0;
}
