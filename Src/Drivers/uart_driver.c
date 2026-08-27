/*
 * uart_driver.c
 *
 *  Created on: Aug 17, 2026
 *      Author: octav
 */

#include "uart_driver.h"

void USART_PeriClockControl(USARTx_Reg_t *pUSARTx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pUSARTx == USART1) USART1_PCLK_EN();
		else if(pUSARTx == USART2) USART2_PCLK_EN();
		else if(pUSARTx == USART6) USART6_PCLK_EN();
	}
	//TODO DISABLE
}

void USART_Init(USART_Handle_t *pUSARTHandle){
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	//USART_CR1
	//MODE
	if(pUSARTHandle->USART_PinConfig.USARTx_Mode == USART_MODE_ONLY_TX){
		pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_RE);
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_TE);
	}
	else if(pUSARTHandle->USART_PinConfig.USARTx_Mode == USART_MODE_ONLY_RX){
		pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_TE);
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_RE);
	}else if(pUSARTHandle->USART_PinConfig.USARTx_Mode == USART_MODE_TXRX){
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_RE);
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_TE);
	}

	if(pUSARTHandle->USART_PinConfig.USARTx_RXInterruptEnable == USART_IRQ_ENABLE){
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_RXNEIE);
	}
	else
	{
		pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_RXNEIE);
	}

	//WORD LENGHT
	if(pUSARTHandle->USART_PinConfig.USARTx_WordLength == USART_WORDLEN_8BITS){
		pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_M);
	}else
	{
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_M);
	}

	//PARITY
	if(pUSARTHandle->USART_PinConfig.USARTx_ParityCtrl != USART_PARITY_DISABLE){
		pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_PCE);
		//ODD
		if(pUSARTHandle->USART_PinConfig.USARTx_ParityCtrl == USART_PARITY_EN_ODD){
			pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_PS);

		}else{ //EVEN
			pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_PS);
		}
	}else
	{
		pUSARTHandle->pUSARTx->CR1 &= ~(1U << USART_CR1_PCE);
	}

	//USART_CR2
	//STOP BITS
	if(pUSARTHandle->USART_PinConfig.USARTx_StopBits == USART_STOPBITS_1)
	{
		pUSARTHandle->pUSARTx->CR2 &= ~(1U << USART_CR2_STOP);
	}
	//TODO STOP BITS

	//BRR
	uint32_t usrdiv = (SYSTICK_CLOCK  * 25 / (4 * pUSARTHandle->USART_PinConfig.USARTx_BaudRate));
	uint32_t mantissa = usrdiv / 100;
	usrdiv -= mantissa * 100;
	uint32_t fraction = ((usrdiv * 16) + 50) / 100;
	if(fraction >= 16){
		mantissa += 1;
		fraction = 0;
	}

	uint16_t baudrate = ((mantissa << 4) | fraction);

	pUSARTHandle->pUSARTx->BRR = baudrate;

	pUSARTHandle->pUSARTx->CR1 |= (1U << USART_CR1_UE);
}

void USART_SendData(USART_Handle_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len){

	while(Len != 0){
		while(!(pUSARTx->pUSARTx->SR & (1U << USART_SR_TXE)));
		pUSARTx->pUSARTx->DR = *pTxBuffer++;
		Len--;
	}
	while(!(pUSARTx->pUSARTx->SR & (1U << USART_SR_TC)));
}

void USART_ReceiveData(USART_Handle_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len){
	while(Len != 0){
		while(!(pUSARTx->pUSARTx->SR & (1U << USART_SR_RXNE)));
		*pRxBuffer++ = (uint8_t)pUSARTx->pUSARTx->DR;
		Len--;
	}
}
