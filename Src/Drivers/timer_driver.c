/*
 * timer_driver.c
 *
 *  Created on: Aug 18, 2026
 *      Author: octav
 */

#include "timer_driver.h"

void TIMx_PeriClockControl(TIMx_Reg_t *pTIMx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pTIMx == TIM9) RCC->APB2ENR |= (1 << 16);
		else if(pTIMx == TIM10)	RCC->APB2ENR |= (1 << 17);
		else if(pTIMx == TIM11)	RCC->APB2ENR |= (1 << 18);
		else if(pTIMx == TIM1) 	RCC->APB2ENR |= (1 << 0);
		else if(pTIMx == TIM2) 	RCC->APB1ENR |= (1 << 0);
		else if(pTIMx == TIM3) 	RCC->APB1ENR |= (1 << 1);
		else if(pTIMx == TIM4) 	RCC->APB1ENR |= (1 << 2);
		else if(pTIMx == TIM5) 	RCC->APB1ENR |= (1 << 3);
	}
	//TODO DISABLE
}

void TIMx_Init(TIMx_Handle_t *pTIMHandle){
	TIMx_PeriClockControl(pTIMHandle->pTIMx, ENABLE);
	pTIMHandle->pTIMx->PSC = pTIMHandle->TIMx_Config.TIMx_Prescaler;
	pTIMHandle->pTIMx->ARR = pTIMHandle->TIMx_Config.TIMx_AutoReload;

	if(pTIMHandle->TIMx_Config.TIMx_UpdateInterruptEnable == TIM_UIE_ENABLE){
		pTIMHandle->pTIMx->DIER |= (1U << TIMx_DIER_UIE);
	}

	pTIMHandle->pTIMx->SR &= ~(1U << TIMx_SR_UIF);

	pTIMHandle->pTIMx->CR1 |= (1U << TIMx_CR1_CEN);
}
