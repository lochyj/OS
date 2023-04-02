#pragma once

#include "system.h"
#include "types.h"

#define FS_FILE		    0x01
#define FS_DIRECTORY	0x02
#define FS_CHARDEVICE	0x03
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE		    0x05
#define FS_SYMLINK	    0x06
#define FS_MOUNTPOINT	0x08

struct fs_node;

typedef uint32_t (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent* (*readdir_type_t)(struct fs_node*, uint32_t);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*, char* name);

// The definition of a filesystem node
typedef struct fs_node {
	signed char name[128];	// The filename
	uint32_t inode;	        // The node ID
	uint32_t flags;	        // The node type
	uint32_t mask;	        // The permissions mask
	uint32_t uid;	        // The owner ID
	uint32_t gid;	        // The group ID
	uint32_t length;	    // The length
	uint32_t impl;	        // Implementation-specific number
	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;
	struct fs_node* ptr;	// Used by mountpoints and symlinks
} fs_node_t;

/* The structure of a directory entry */
struct dirent {
	char name[128];
	uint32_t inode;	/* The node ID */
};

// Defined when we init initrd
extern struct fs_node* fs_root;

uint32_t read_fs(struct fs_node* node, uint32_t offset, uint32_t size, uint8_t* buffer);
uint32_t write_fs(struct fs_node* node, uint32_t offset, uint32_t size, uint8_t* buffer);
void open_fs(struct fs_node* node, uint8_t read, uint8_t write);
void close_fs(struct fs_node* node);
struct dirent* readdir_fs(struct fs_node* node, uint32_t index);
struct fs_node* finddir_fs(struct fs_node* node, char* name);
