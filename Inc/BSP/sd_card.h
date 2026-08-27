/*
 * sd_card.h
 *
 *  Created on: Aug 18, 2026
 *      Author: octav
 */

#ifndef BSP_SD_CARD_H_
#define BSP_SD_CARD_H_

#include "spi_driver.h"
#include "gpio_driver.h"

typedef enum
{
	cmd_ok,
	cmd0_fail,
	cmd8_fail,
	cmd55_fail,
	cmd41_fail,
	cmd58_fail,
	cmd17_fail,
	cmd18_fail,
	cmd12_fail,
	timeout_error
}sd_status_e;

typedef struct
{
	SPI_Handle_t *pSPIHandle;
	GPIO_Handle_t *pCSHandle;
	uint8_t CardType;
}SD_Handle_t;

#define SD_CMD0 	0x00
#define SD_CMD8 	0x08
#define SD_CMD55 	0x37
#define SD_ACMD41 	0x29
#define SD_CMD58 	0x3A
#define SD_CMD17 	0x11
#define SD_CMD12 	0x0C
#define SD_CMD18 	0x12

#define SD_CRC0 0x95
#define SD_CRC7 0x87

#define SD_R1_IDLE_STATE 0x01
#define SD_R1_READY_STATE 0x00
#define SD_R7 0x1AA

#define SD_DATA_TOKEN 0xFE
#define SD_NOT_BUSY 0xFF

#define SD_SDSC 0
#define SD_SDHC 1

#define SD_BPB		0x00

#define SD_FAT		0xC4E
#define SD_DATA_AREA 0x8000
uint8_t SD_SendCommand(SD_Handle_t *pSDHandle, uint8_t cmd, uint32_t args, uint8_t crc);
sd_status_e SD_Init(SD_Handle_t *pSDHandle);
sd_status_e SD_ReadBlock(SD_Handle_t *pSDHandle, uint32_t blockNumber, uint8_t* buffer);
sd_status_e SD_ReadMultBlocks(SD_Handle_t *pSDHandle, uint32_t blockNumber, uint8_t* buffer, uint32_t blockCount);
#endif /* BSP_SD_CARD_H_ */
