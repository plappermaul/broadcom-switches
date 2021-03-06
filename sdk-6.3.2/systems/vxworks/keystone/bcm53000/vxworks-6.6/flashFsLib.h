#ifndef FLASH_FS_LIB_H
#define FLASH_FS_LIB_H

/* $Id: flashFsLib.h,v 1.7 2012/03/02 15:28:07 yaronm Exp $
 * Support for Flash Filesystem and Flash Boot Area
 * Built on top of flashDrvLib.
 */

STATUS flashFsLibInit(void);
STATUS flashFsSync(void);

extern int flsh_boot_size;

#define FLASH_BOOT_START       0x000000
#define FLASH_BOOT_START_SECTOR    (FLASH_BOOT_START / FLASH_SECTOR_SIZE)
#define FLASH_BOOT_SIZE        (flsh_boot_size * 1024)
#define FLASH_BOOT_SIZE_SECTORS    (FLASH_BOOT_SIZE / FLASH_SECTOR_SIZE)

#define FLASH_NVRAM_SIZE    (128 * 1024)
#define FLASH_NVRAM_START    FLASH_BOOT_SIZE
#define FLASH_NVRAM_START_SECTOR   (FLASH_NVRAM_START / FLASH_SECTOR_SIZE)

#define FLASH_FS_BLOCK_SIZE    512

#define FLASH_FS_SIZE    (FLASH_SIZE - FLASH_BOOT_SIZE - FLASH_NVRAM_SIZE)
#define FLASH_FS_SIZE_BLOCKS       (FLASH_FS_SIZE / FLASH_FS_BLOCK_SIZE)
#define FLASH_FS_BLOCK_PER_SECTOR  (FLASH_SECTOR_SIZE / FLASH_FS_BLOCK_SIZE)

#define FLASH_FS_NAME   "flash:"

typedef enum {

    FLASH_FS_SYNC = 0x10000

} FLASH_FS_CUSTOM_IOCTL_DEFINITIONS;

#endif
