//* ------------------------------------------------------------------------------------------------------------------------------
//* The following code came from https://github.com/nanobyte-dev/nanobyte_os/releases/tag/Part6                                  |                    
//* It has been modified by me to work with my kernel and to fit my needs.                                                       |
//* The non-modified parts of the code is licensed under the Unlicense, which is a public domain license.                        |
//* The modified parts of the code is licensed under the MIT licence and / or in accordance to the root licence of the project.  |
//* ------------------------------------------------------------------------------------------------------------------------------
#include <stdint.h>

#ifndef DISK_H

#define DISK_H

bool x86_Disk_Reset(u8 drive);

bool x86_Disk_Read(
    u8 drive,
    u16 cylinder,
    u16 sector,
    u16 head,
    u8 count,
    void far * dataOut
);

bool x86_Disk_GetDriveParams(
    u8 drive,
    u8* driveTypeOut,
    u16* cylindersOut,
    u16* sectorsOut,
    u16* headsOut
);

#endif
