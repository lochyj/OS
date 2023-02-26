//* ------------------------------------------------------------------------------------------------------------------------------
//* The following code came from https://github.com/nanobyte-dev/nanobyte_os/releases/tag/Part6                                  |                    
//* It has been modified by me to work with my kernel and to fit my needs.                                                       |
//* The non-modified parts of the code is licensed under the Unlicense, which is a public domain license.                        |
//* The modified parts of the code is licensed under the MIT licence and / or in accordance to the root licence of the project.  |
//* ------------------------------------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>

#include "disk.asm"

#ifndef DISK_H

#define DISK_H

extern bool x86_Disk_Reset(u8 drive);

extern bool x86_Disk_Read (
    u8 drive,
    u16 cylinder,
    u16 sector,
    u16 head,
    u8 count,
    void* dataOut
);

extern bool x86_Disk_GetDriveParams (
    u8 drive,
    u8* driveTypeOut,
    u16* cylindersOut,
    u16* sectorsOut,
    u16* headsOut
);

typedef struct {
    u8 id;
    u16 cylinders;
    u16 sectors;
    u16 heads;
} DISK;

bool DISK_Initialize(DISK* disk, uint8_t driveNumber)
{
    uint8_t driveType;
    uint16_t cylinders, sectors, heads;

    if (!x86_Disk_GetDriveParams(disk->id, &driveType, &cylinders, &sectors, &heads))
        return false;

    disk->id = driveNumber;
    disk->cylinders = cylinders + 1;
    disk->heads = heads + 1;
    disk->sectors = sectors;

    return true;
}

void DISK_LBA2CHS(DISK* disk, uint32_t lba, uint16_t* cylinderOut, uint16_t* sectorOut, uint16_t* headOut)
{
    // sector = (LBA % sectors per track + 1)
    *sectorOut = lba % disk->sectors + 1;

    // cylinder = (LBA / sectors per track) / heads
    *cylinderOut = (lba / disk->sectors) / disk->heads;

    // head = (LBA / sectors per track) % heads
    *headOut = (lba / disk->sectors) % disk->heads;
}

bool DISK_ReadSectors(DISK* disk, uint32_t lba, uint8_t sectors, void* dataOut)
{
    uint16_t cylinder, sector, head;

    DISK_LBA2CHS(disk, lba, &cylinder, &sector, &head);

    for (int i = 0; i < 3; i++)
    {
        if (x86_Disk_Read(disk->id, cylinder, sector, head, sectors, dataOut))
            return true;

        x86_Disk_Reset(disk->id);
    }

    return false;
}

#endif
