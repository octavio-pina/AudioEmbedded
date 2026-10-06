/*
 * fat32.c
 *
 *  Created on: Aug 20, 2026
 *      Author: octav
 */

#include "fat32.h"

//static uint8_t buffer[512];
static uint8_t fatCache[512];
static uint32_t cachedFATSector = 0xFFFFFFFF;

FS_Status_e Init_Filesystem(SD_Handle_t *sd, FileSystem_t *fs){


	fs->SDHandler = sd;
	if(SD_ReadBlock(sd, SD_BPB, fatCache) != cmd_ok){
		return error;
	}


	fs->BytesPerSector = fatCache[BPB_BYTSPERSEC_OFF + 1] << 8 |
			fatCache[BPB_BYTSPERSEC_OFF];

	fs->SecPerClus = fatCache[BPB_SECPERCLUS_OFF];

	fs->FATStart =	(uint32_t)fatCache[BPB_RSVDSECCNT_OFF + 1] << 8 |
						(uint32_t)fatCache[BPB_RSVDSECCNT_OFF];

	fs->NumFATs = fatCache[BPB_NUMFATS_OFF];

	fs->RootClus = 	(uint32_t)fatCache[BPB_ROOTCLUS_OFF + 3] << 24 | 
					(uint32_t)fatCache[BPB_ROOTCLUS_OFF + 2] << 16 |
					(uint32_t)fatCache[BPB_ROOTCLUS_OFF + 1] << 8  |
					(uint32_t)fatCache[BPB_ROOTCLUS_OFF + 0] << 0  ;

	fs->FATSize = 	(uint32_t)fatCache[BPB_FATSZ32_OFF + 3] << 24 | 
					(uint32_t)fatCache[BPB_FATSZ32_OFF + 2] << 16 |
					(uint32_t)fatCache[BPB_FATSZ32_OFF + 1] << 8  |
					(uint32_t)fatCache[BPB_FATSZ32_OFF + 0] << 0  ;

	fs->FirstDataSector = fs->FATStart + (fs->FATSize * fs->NumFATs);

	return fs_ok;
}
FS_Status_e ClusterToSector(FileSystem_t *fs, uint32_t cluster, uint32_t *sector){

	if(sector == NULL || fs == NULL){
		return error;
	}

	if(cluster < 2){
		return cluster_error;
	}

	*sector = fs->FirstDataSector + ((cluster - 2) * fs->SecPerClus);

	return fs_ok;

}
FS_Status_e GetNextCluster(FileSystem_t *fs, uint32_t currentCluster, uint32_t *nextCluster){

	if(nextCluster == NULL || fs == NULL){
		return error;
	}

	uint32_t entriesPerSector = fs->BytesPerSector / FAT32_ENTRY_SIZE;

	uint32_t relativeFATSector = currentCluster / entriesPerSector;
	uint32_t entryIndex = currentCluster % entriesPerSector;

	uint32_t FATSector = fs->FATStart + relativeFATSector;
	uint32_t entryOffset = entryIndex * FAT32_ENTRY_SIZE;

	if(FATSector != cachedFATSector){
		if(SD_ReadBlock(fs->SDHandler, FATSector, fatCache) != cmd_ok)
		{
			return error;
		}
		cachedFATSector = FATSector;
	}

	uint32_t temp = (uint32_t)fatCache[entryOffset + 3] << 24 | 					(uint32_t)fatCache[entryOffset + 2] << 16 |
					(uint32_t)fatCache[entryOffset + 1] << 8  |
					(uint32_t)fatCache[entryOffset + 0] << 0  ;
	temp &= 0x0FFFFFFF;
	if(temp >= 0x0FFFFFF8){
		return fs_eoc;
	}
	else
	{
		*nextCluster = temp;
	}
	return fs_ok;
}
FS_Status_e FindFile(FileSystem_t *fs, const char *name83, uint32_t *FileSize, uint32_t *FirstCluster){
	uint32_t RootDir;

	if (name83 == NULL || fs == NULL || FileSize == NULL || FirstCluster == NULL){
		return error;
	}

	if(ClusterToSector(fs, fs->RootClus, &RootDir) != fs_ok)
	{
		return cluster_error;
	}

	if(SD_ReadBlock(fs->SDHandler, RootDir, fatCache) != cmd_ok)
	{
		return error;
	}

	uint32_t entriesPerSector = fs->BytesPerSector / FAT32_ROOTENTRY_SIZE;
	uint32_t entryFind = 0xFFFFFFFF;
	uint8_t cnt = 0;

	for(int i = 0; i < entriesPerSector; i++){
		cnt = 0;
		for(int j = 0; j < 11; j++){
			if(fatCache[(i * FAT32_ROOTENTRY_SIZE) + j] != name83[j]){
				break;
			}
			cnt++;
		}
		if(cnt == 11){
			entryFind = i;
			break;
		}
	}

	if(entryFind == 0xFFFFFFFF){
		return file_not_found;
	}

	entryFind *= FAT32_ROOTENTRY_SIZE;

	*FirstCluster = (uint32_t)fatCache[entryFind + DIR_FIRSTCLUS_HI_OFF + 1] << 24 | 					(uint32_t)fatCache[entryFind + DIR_FIRSTCLUS_HI_OFF + 0] << 16 |
					(uint32_t)fatCache[entryFind + DIR_FIRSTCLUS_LO_OFF + 1] << 8  |
					(uint32_t)fatCache[entryFind + DIR_FIRSTCLUS_LO_OFF + 0] << 0  ;

	*FileSize = (uint32_t)fatCache[entryFind + DIR_FILESIZE_OFF + 3] << 24 | 				(uint32_t)fatCache[entryFind + DIR_FILESIZE_OFF + 2] << 16 |
				(uint32_t)fatCache[entryFind + DIR_FILESIZE_OFF + 1] << 8  |
				(uint32_t)fatCache[entryFind + DIR_FILESIZE_OFF + 0] << 0  ;

	return fs_ok;
}

FS_Status_e OpenFile(FileSystem_t *fs, File_t *file, const char *name83){

	if (name83 == NULL || fs == NULL || file == NULL){
		return error;
	}
	if(FindFile(fs, name83, &file->FileSize, &file->FirstCluster) != fs_ok){
		return file_not_found;
	}

	file->fs = fs;
	file->currentCluster = file->FirstCluster;
	file->indexSector = 0;
	file->remainingFileSize = file->FileSize;

	return fs_ok;
}

FS_Status_e ReadNextBlock(File_t *file, uint8_t *buffer, uint16_t *validBytes){
	if (file == NULL || buffer == NULL || validBytes == NULL){
		return error;
	}

	if(file->remainingFileSize == 0){
		return fs_eoc;
	}

	if(file->indexSector == file->fs->SecPerClus){
		uint32_t nextCluster;
		if(GetNextCluster(file->fs, file->currentCluster, &nextCluster) != fs_ok){
			return error;
		}
		else{
			file->currentCluster = nextCluster;
			file->indexSector = 0;
		}

	}

	uint32_t firstSector = 0;
	if(ClusterToSector(file->fs, file->currentCluster, &firstSector) != fs_ok){
		return error;
	}

	if(SD_ReadBlock(file->fs->SDHandler, firstSector + file->indexSector, buffer) != cmd_ok){
		return error;
	}

	if(file->remainingFileSize >= file->fs->BytesPerSector){
		*validBytes = file->fs->BytesPerSector;
	}
	else
	{
		*validBytes = file->remainingFileSize;
	}

	file->remainingFileSize -= *validBytes;

	file->indexSector++;

	return fs_ok;

}
