/*
 * spi_driver.c
 *
 *  Created on: Aug 18, 2026
 *      Author: octav
 */

#include "spi_driver.h"

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1) RCC->APB2ENR |= (1U << 12);
		else if(pSPIx == SPI2) RCC->APB1ENR |= (1U << 14);
		else if(pSPIx == SPI3) RCC->APB1ENR |= (1U << 15);
		else if(pSPIx == SPI4) RCC->APB2ENR |= (1U << 13);
		else if(pSPIx == SPI5) RCC->APB2ENR |= (1U << 20);
	}
	//TODO DISABLE
}

void SPI_Init(SPI_Handle_t *pSPIHandle){
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_DEVICE_MODE_MASTER){
		pSPIHandle->pSPIx->CR1 |= (1U << SPI_CR1_MSTR);
	}
	else{
		pSPIHandle->pSPIx->CR1 &= ~(1U << SPI_CR1_MSTR);
	}

	//CLK Speed
	pSPIHandle->pSPIx->CR1 &= ~(0x7U << SPI_CR1_BR);
	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

	//CPOL & CPHA
	pSPIHandle->pSPIx->CR1 &= ~(1U << SPI_CR1_CPOL);
	pSPIHandle->pSPIx->CR1 &= ~(1U << SPI_CR1_CPHA);
	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);
	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);

	//DFF
	pSPIHandle->pSPIx->CR1 &= ~(1U << SPI_CR1_DFF);
	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

	//LSBFIRST
	pSPIHandle->pSPIx->CR1 &= ~(1U << SPI_CR1_LSBFIRST);
	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.SPI_FirstBit << SPI_CR1_LSBFIRST);

	//SSM
	if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN){
		pSPIHandle->pSPIx->CR1 |= (1U << SPI_CR1_SSM);
		pSPIHandle->pSPIx->CR1 |= (1U << SPI_CR1_SSI);

	}
	else{
		pSPIHandle->pSPIx->CR1 &= ~(1U << SPI_CR1_SSM);
	}

	pSPIHandle->pSPIx->CR1 |= (1U << SPI_CR1_SPE);

}

uint8_t SPI_TransferByte(SPI_RegDef_t *pSPIx, uint8_t txByte){

	while(!(pSPIx->SR & (1U << SPI_SR_TXE)));

	pSPIx->DR = (uint16_t)txByte;

	while(!(pSPIx->SR & (1U << SPI_SR_RXNE)));

	return (uint8_t)pSPIx->DR;
}

bool SPI_ReceiveBuffer(SPI_RegDef_t *pSPIx, uint8_t *buffer, uint16_t length){

	uint32_t timeout=0;
	for(uint16_t i = 0; i < length; i++){
		while(!(pSPIx->SR & (1U << SPI_SR_TXE))){
			if(timeout >= SPI_MAX_WAIT){
				return false;
			}
			timeout++;
		}
		timeout=0;

		pSPIx->DR = 0xFF;

		while(!(pSPIx->SR & (1U << SPI_SR_RXNE))){
			if(timeout >= SPI_MAX_WAIT){
				return false;
			}
			timeout++;
		}

		buffer[i] = (uint8_t)pSPIx->DR;
		timeout=0;
	}

	return true;
}
