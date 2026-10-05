/*
 * wav.c
 *
 *  Created on: Aug 23, 2026
 *      Author: octav
 */


#include "wav.h"
#include <stdint.h>
static uint8_t buffer[512] = {};
static uint8_t frameBuffer[4] = {};
static uint8_t frameIndex = 0;

static inline bool CompareTag(uint16_t entryOffset, const char *tag, uint8_t* bufferWav){
	for (int i = 0; i < 4; i++) {
		if(bufferWav[entryOffset + i] != tag[i])
			return false;
	}
	return true;
}

static inline uint32_t ReadLE32(uint8_t off){
	uint32_t tmp = 0;
	tmp |= (uint32_t)buffer[off] 			|
			(uint32_t)buffer[off+1] << 8 	|
			(uint32_t)buffer[off+2] << 16 	|
			(uint32_t)buffer[off+3] << 24;
	return tmp;
}
static inline uint16_t ReadLE16(uint8_t off){
	uint16_t tmp = 0;
	tmp |= (uint16_t)buffer[off] |
			(uint16_t)buffer[off+1] << 8;
	return tmp;
}

WAV_Status_e WAV_Open(File_t *file, WAV_t *wav){
	if(file == NULL || wav == NULL){
		return wav_error;
	}

	wav->file = file;

	if( ReadNextBlock(file, buffer, &wav->validBytes) != fs_ok){
		return wav_error;
	}

	if(!CompareTag(0, "RIFF", buffer)){
		return wav_invalid;
	}

	if(!CompareTag(8, "WAVE", buffer)){
		return wav_invalid;
	}
	uint32_t initialOff = 0x0C;
	bool dataFlag = false;
	uint8_t tryout = 0;
	while(!dataFlag && tryout < 4){
		uint32_t off = initialOff;

		uint32_t ChunkID = ReadLE32(off + 0);
		uint32_t SizeFormatChunk = ReadLE32(off + 4);
		if(ChunkID == WAV_CHUNK_FMT){
			wav->AudioFormat = ReadLE16(off + 8);
			wav->NumChannels = ReadLE16(off + 10);
			wav->SampleRate = ReadLE32(off + 12);
			wav->ByteRate = ReadLE32(off + 16);
			wav->BlockAlign = ReadLE16(off + 20);
			wav->BitsPerSample = ReadLE16(off + 22);
		}
		else if(ChunkID == WAV_CHUNK_DATA)
		{
			wav->DataSize = SizeFormatChunk;
			wav->startData = initialOff + 8;
			wav->bufferOffset = wav->startData;
			wav->remainingData = wav->DataSize;
			dataFlag = true;
		}
		initialOff += 8 + SizeFormatChunk;
		tryout++;
	}

	if(tryout >= 4){
		return wav_error;
	}

	return wav_ok;
}

Frame_Status_e WAV_ReadFrame(WAV_t *wav, PCM_Frame_t* frame){
	if(frame == NULL || wav == NULL){
		return frame_error;
	}
	
	while(frameIndex < 4){

		if(wav->remainingData == 0){
			if(frameIndex == 0){
				return frame_eof;
			}
			return frame_error;
		}

		if(wav->bufferOffset >= wav->validBytes){
			if( ReadNextBlock(wav->file, buffer, &wav->validBytes) != fs_ok){
				return frame_error;
			}

			if(wav->validBytes == 0){
				return frame_error;
			}
			wav->bufferOffset = 0;
		}

		frameBuffer[frameIndex++] = buffer[wav->bufferOffset++];
		wav->remainingData--;

	}

	frameIndex = 0;

	frame->left_frame = frameBuffer[1] << 8 | frameBuffer[0];
	frame->rigth_frame = frameBuffer[3] << 8 | frameBuffer[2];
	return frame_ok;
}