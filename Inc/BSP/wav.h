/*
 * wav.h
 *
 *  Created on: Aug 23, 2026
 *      Author: octav
 */

#ifndef BSP_WAV_H_
#define BSP_WAV_H_

#include "fat32.h"
typedef enum{
	wav_ok,
	wav_error,
	wav_invalid
}WAV_Status_e;

typedef struct
{
	File_t *file;

	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint16_t ByteRate;
	uint8_t BlockAlign;
	uint16_t BitsPerSample;

	uint32_t DataSize;
	uint32_t startData;


}WAV_t;

#define WAV_CHUNK_FMT 	0x20746D66
#define WAV_CHUNK_DATA	0x61746164

WAV_Status_e WAV_Open(File_t *file, WAV_t *wav);

#endif /* BSP_WAV_H_ */
