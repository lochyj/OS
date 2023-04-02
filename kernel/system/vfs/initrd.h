#include "kernel/system/vfs/vfs.h"
#include "system.h"
#include "types.h"
#include "debug.h"
#include "kernel/system/vmem/memory.h"

struct initrd_header {
	uint32_t nfiles;	    // Number of files in the ramdisk
};

struct initrd_file_header {
	uint8_t magic;	        // Magic number, used for error checking
	signed char name[64];
	uint32_t offset;	    // Offset in the initrd that the file starts
	uint32_t length;
};

/* Initializing the initial ramdisk. It gets passed the address of the
multiboot module and returns a completed filesystem node*/
struct fs_node* initrd_install(uint32_t location);