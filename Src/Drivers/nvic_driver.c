/*
 * nvic_driver.c
 *
 *  Created on: Aug 18, 2026
 *      Author: octav
 */


#include "stm32f411xx.h"

void NVIC_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(IRQNumber <= 31) *NVIC_ISER0 = (1U << IRQNumber);
		else if (IRQNumber > 31 && IRQNumber < 64)		*NVIC_ISER1 = (1U << (IRQNumber % 32));
		else if (IRQNumber >= 64 && IRQNumber < 96) 	*NVIC_ISER2 = (1U << (IRQNumber % 64));
	}
	else
	{
		if(IRQNumber <= 31) *NVIC_ICER0 = (1U << IRQNumber);
		else if (IRQNumber > 31 && IRQNumber < 64)		*NVIC_ICER1 = (1U << (IRQNumber % 32));
		else if (IRQNumber >= 64 && IRQNumber < 96) 	*NVIC_ICER2 = (1U << (IRQNumber % 64));
	}
}

void NVIC_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	uint8_t reg_idx = IRQNumber >> 2;
	uint8_t irq_idx = IRQNumber % 4;


	NVIC_IPR_ADDR[reg_idx] &= ~(0xFF << (irq_idx * 8));
	NVIC_IPR_ADDR[reg_idx] |= ((IRQPriority << 4) << (irq_idx * 8));
}
