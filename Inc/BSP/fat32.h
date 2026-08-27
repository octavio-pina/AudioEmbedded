/*
 * fat32.h
 *
 *  Created on: Aug 20, 2026
 *      Author: octav
 */

#ifndef BSP_FAT32_H_
#define BSP_FAT32_H_

#include "sd_card.h"

typedef enum{
	fs_ok,
	error,
	fs_eoc,
	file_not_found,
	cluster_error
}FS_Status_e;


typedef struct
{
	SD_Handle_t *SDHandler;

	uint16_t BytesPerSector;
	uint8_t SecPerClus;
	uint32_t FATStart;
	uint8_t NumFATs;
	uint32_t FATSize;
	uint32_t RootClus;
	uint32_t FirstDataSector;
}FileSystem_t;

typedef struct
{
	FileSystem_t *fs;

	uint32_t FirstCluster;
	uint32_t currentCluster;
	uint32_t indexSector;
	uint32_t FileSize;
	uint32_t remainingFileSize;
}File_t;

#define BPB_BYTSPERSEC_OFF 	0x0B
#define BPB_SECPERCLUS_OFF 	0x0D
#define BPB_RSVDSECCNT_OFF 	0x0E
#define BPB_NUMFATS_OFF 		0x10
#define BPB_FATSZ32_OFF 		0x24
#define BPB_ROOTCLUS_OFF 		0x2C

#define DIR_FILESIZE_OFF 0x1C
#define DIR_FIRSTCLUS_HI_OFF 0x14
#define DIR_FIRSTCLUS_LO_OFF 0x1A

#define FAT32_ENTRY_SIZE  4U
#define FAT32_ROOTENTRY_SIZE  32U

FS_Status_e Init_Filesystem(SD_Handle_t *sd, FileSystem_t *fs);
FS_Status_e ClusterToSector(FileSystem_t *fs, uint32_t cluster, uint32_t *sector);
FS_Status_e GetNextCluster(FileSystem_t *fs, uint32_t currentCluster, uint32_t *nextCluster);
FS_Status_e FindFile(FileSystem_t *fs, const char *name83, uint32_t *FileSize, uint32_t *FirstCluster);
FS_Status_e OpenFile(FileSystem_t *fs, File_t *file, const char *name83);
FS_Status_e ReadNextBlock(File_t *file, uint8_t *buffer, uint16_t *validBytes);
#endif /* BSP_FAT32_H_ */
