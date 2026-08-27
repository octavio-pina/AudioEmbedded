/*
 * sd_card.c
 *
 *  Created on: Aug 18, 2026
 *      Author: octav
 */

#include "sd_card.h"

static void SD_CS_Low(SD_Handle_t *pSDHandle)
{
    GPIO_WriteToOutputPin(
        pSDHandle->pCSHandle->pGPIOx,
        pSDHandle->pCSHandle->GPIO_PinConfig.GPIO_PinNum,
        RESET
    );
}
static void SD_CS_High(SD_Handle_t *pSDHandle)
{
    GPIO_WriteToOutputPin(
        pSDHandle->pCSHandle->pGPIOx,
        pSDHandle->pCSHandle->GPIO_PinConfig.GPIO_PinNum,
        SET
    );
}

uint8_t SD_SendCommand(SD_Handle_t *pSDHandle, uint8_t cmd, uint32_t args, uint8_t crc){
	uint8_t txByte = 0;
	uint16_t timeout = 100;
	uint8_t response = 0xFF;
	txByte |= ((1U << 6) | cmd);
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, txByte);

	txByte = (args >> 24) & 0xFF;
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, txByte);
	txByte = (args >> 16) & 0xFF;
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, txByte);
	txByte = (args >> 8)  & 0xFF;
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, txByte);
	txByte = (args >> 0)  & 0xFF;
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, txByte);

	txByte = crc;
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, txByte);

	if(cmd == SD_CMD12){
		SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF); //Stuff Byte
	}

	while(timeout > 0){
		response = SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
		if(!(response & (1 << 7))){
			return response;
		}
		timeout--;
	}

	return response;
}

sd_status_e SD_Init(SD_Handle_t *pSDHandle){
	uint8_t initclk = 10;
	uint32_t response = 0;
	uint8_t cntBytes = 4;
	bool ready_flag = false;
	uint8_t timeout = 100;

	SD_CS_High(pSDHandle);
	while(initclk-- > 0){
		SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
	}

	//CMD0
	SD_CS_Low(pSDHandle);
	response = SD_SendCommand(pSDHandle, SD_CMD0, 0x00000000, SD_CRC0);
	SD_CS_High(pSDHandle);
	if(response != SD_R1_IDLE_STATE){
		return cmd0_fail;
	}

	//CMD8
	SD_CS_Low(pSDHandle);
	response = SD_SendCommand(pSDHandle, SD_CMD8, 0x000001AA, SD_CRC7);

	if(response != SD_R1_IDLE_STATE){
		SD_CS_High(pSDHandle);
		return cmd8_fail;
	}
	response = 0;
	while(cntBytes > 0){
		response <<= 8;
		response |= SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
		cntBytes--;
	}
	SD_CS_High(pSDHandle);
	if((response & (0xFFF)) != SD_R7){
		return cmd8_fail;
	}

	//CMD55 + ACMD41
	while(!ready_flag && timeout > 0){
		SD_CS_Low(pSDHandle);
		response = SD_SendCommand(pSDHandle, SD_CMD55, 0x00000000, SD_CRC0);
		SD_CS_High(pSDHandle);
		if(response != SD_R1_IDLE_STATE){
			return cmd55_fail;
		}

		SD_CS_Low(pSDHandle);
		response = SD_SendCommand(pSDHandle, SD_ACMD41, 0x40000000, SD_CRC0);
		SD_CS_High(pSDHandle);
		switch(response){
		case SD_R1_READY_STATE:
			ready_flag = true;
			break;
		case SD_R1_IDLE_STATE:
			timeout--;
			break;
		default:
			return cmd41_fail;
		}
	}

	if(!ready_flag){
		return cmd41_fail;
	}

	//CMD58
	cntBytes = 4;
	SD_CS_Low(pSDHandle);
	response = SD_SendCommand(pSDHandle, SD_CMD58, 0x00000000, SD_CRC7);

	if(response != SD_R1_READY_STATE){
		SD_CS_High(pSDHandle);
		return cmd58_fail;
	}
	response = 0;
	while(cntBytes > 0){
		response <<= 8;
		response |= SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
		cntBytes--;
	}
	SD_CS_High(pSDHandle);
	if(((response >> 30) & 0x1) == SD_SDSC){
		pSDHandle->CardType = SD_SDSC;
	}
	else{
		pSDHandle->CardType = SD_SDHC;

	}

	return cmd_ok;
}

sd_status_e SD_ReadBlock(SD_Handle_t *pSDHandle, uint32_t blockNumber, uint8_t* buffer){

	uint8_t timeout = 100;
	bool token_flag = false;

	if(pSDHandle->CardType == SD_SDSC){
		blockNumber *= 512;
	}

	SD_CS_Low(pSDHandle);
	if(SD_SendCommand(pSDHandle, SD_CMD17, blockNumber, SD_CRC7) != SD_R1_READY_STATE){
		SD_CS_High(pSDHandle);
		return cmd17_fail;
	}

	while(!token_flag && timeout > 0){
		if(SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF) == SD_DATA_TOKEN){
			token_flag = true;
		}
		timeout--;
	}

	if(!token_flag){
		SD_CS_High(pSDHandle);
		return timeout_error;
	}

//	for(uint16_t i = 0; i < 512; i++){
//		buffer[i] = SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
//	}
	SPI_ReceiveBuffer(pSDHandle->pSPIHandle->pSPIx, buffer, 512);

	//LECTURES OF CRC
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
	SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
	SD_CS_High(pSDHandle);

	return cmd_ok;
}

sd_status_e SD_ReadMultBlocks(SD_Handle_t *pSDHandle, uint32_t blockNumber, uint8_t* buffer, uint32_t blockCount){
	uint8_t timeout = 100;
	bool token_flag = false;

	if(pSDHandle->CardType == SD_SDSC){
		blockNumber *= 512;
	}

	SD_CS_Low(pSDHandle);
	if(SD_SendCommand(pSDHandle, SD_CMD18, blockNumber, SD_CRC7) != SD_R1_READY_STATE){
		SD_CS_High(pSDHandle);
		return cmd18_fail;
	}

	for(uint32_t i=0; i < blockCount; i++){
		timeout = 100;
		token_flag = false;

		while(!token_flag && timeout > 0){
			if(SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF) == SD_DATA_TOKEN){
				token_flag = true;
			}
			timeout--;
		}

		if(!token_flag){
			SD_CS_High(pSDHandle);
			return timeout_error;
		}

		if(SPI_ReceiveBuffer(pSDHandle->pSPIHandle->pSPIx, buffer + (i * 512), 512) != true){
			SD_CS_High(pSDHandle);
			return timeout_error;
		}

		//LECTURES OF CRC
		SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
		SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF);
	}
	timeout = 100;
	token_flag = false;

	if(SD_SendCommand(pSDHandle, SD_CMD12, 0x00000000, SD_CRC7) != SD_R1_READY_STATE){
		SD_CS_High(pSDHandle);
		return cmd12_fail;
	}

	while(!token_flag && timeout > 0){
		if(SPI_TransferByte(pSDHandle->pSPIHandle->pSPIx, 0xFF) == SD_NOT_BUSY){
			token_flag = true;
		}
		timeout--;
	}

	if(!token_flag){
		SD_CS_High(pSDHandle);
		return timeout_error;
	}

	SD_CS_High(pSDHandle);
	return cmd_ok;

}
