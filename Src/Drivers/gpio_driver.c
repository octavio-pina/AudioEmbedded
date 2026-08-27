/*
 * gpio_driver.c
 *
 *  Created on: Aug 16, 2026
 *      Author: octav
 */
#include "gpio_driver.h"

static inline uint8_t GPIO_Portcode(GPIOx_Reg_t *pGPIOx)
{
    if      (pGPIOx == GPIOA) return 0;
    else if (pGPIOx == GPIOB) return 1;
    else if (pGPIOx == GPIOC) return 2;
    else if (pGPIOx == GPIOD) return 3;
    else if (pGPIOx == GPIOE) return 4;
    else if (pGPIOx == GPIOH) return 7;
    return 0; // Valor seguro por defecto
}

void GPIO_PeriClockControl(GPIOx_Reg_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE){
		if(pGPIOx == GPIOA)	PA_PCLK_EN();
		else if (pGPIOx == GPIOB) PB_PCLK_EN();
		else if (pGPIOx == GPIOC) PC_PCLK_EN();
		else if (pGPIOx == GPIOD) PD_PCLK_EN();
		else if (pGPIOx == GPIOE) PE_PCLK_EN();
		else if (pGPIOx == GPIOH) PH_PCLK_EN();
	}
	//TODO DISABLE
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle){

	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	pGPIOHandle->pGPIOx->MODER &= ~(3U << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum)); //Clean the 2 bits register

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		pGPIOHandle->pGPIOx->MODER |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum));
	}
	else
	{
		pGPIOHandle->pGPIOx->MODER |= (GPIO_MODE_IN << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum)); //Input Mode
		//Interrupt Mode
		switch(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode){
		case GPIO_MODE_IT_FT:
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			break;
		case GPIO_MODE_IT_RT:
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			break;
		case GPIO_MODE_IT_RFT:
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			break;
		}

		//Configure the GPIO Port in SYSCFG_EXTICR
		uint8_t exti_r = pGPIOHandle->GPIO_PinConfig.GPIO_PinNum >> 2;
		uint8_t exti_p = pGPIOHandle->GPIO_PinConfig.GPIO_PinNum % 4;

		SYSCFG_PCKL_EN();

		SYSCFG->EXTICR[exti_r] &= ~(0xFU << (exti_p * 4));
		SYSCFG->EXTICR[exti_r] |= (GPIO_Portcode(pGPIOHandle->pGPIOx) << (exti_p * 4));

		// Enable the EXTI Interrupt Delivery
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);

	}

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode != GPIO_MODE_ANALOG){
		//Configure the PuPd Settings
		pGPIOHandle->pGPIOx->PUPDR &= ~(3U << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum));
		pGPIOHandle->pGPIOx->PUPDR |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdCtlr << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum));

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_OUT || pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFUN){
			//Configure the Speed
			pGPIOHandle->pGPIOx->OSPEEDR &= ~(3U << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum));
			pGPIOHandle->pGPIOx->OSPEEDR |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNum));

			//Configure the Output Type (Push-Pull || Open-Drain)
			pGPIOHandle->pGPIOx->OTYPER &= ~(1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);
			pGPIOHandle->pGPIOx->OTYPER |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNum);

			if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFUN){
				//Configure the Alt Func
				uint8_t reg_index = pGPIOHandle->GPIO_PinConfig.GPIO_PinNum >> 3;
				uint8_t pin_offset = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNum % 8) * 4;

				pGPIOHandle->pGPIOx->AFR[reg_index] &= ~(0xFU << pin_offset);
				pGPIOHandle->pGPIOx->AFR[reg_index] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << pin_offset);
			}
		}
	}

}

uint8_t  GPIO_ReadFromInputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber){
	uint8_t value = (pGPIOx->IDR >> PinNumber) & 0x1U;
	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIOx_Reg_t *pGPIOx){
	return (uint16_t)pGPIOx->IDR;
}

void GPIO_WriteToOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
	if(Value == GPIO_PIN_HIGH){
		pGPIOx->BSRR = (1U << PinNumber);
	}
	else
	{
		pGPIOx->BSRR = (1U << (16 + PinNumber));
	}
}

void GPIO_WriteToOutputPort(GPIOx_Reg_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= (1U << PinNumber);
}


void GPIO_IRQHandling(uint8_t PinNumber){
	if(EXTI->PR & (1 << PinNumber))
		EXTI->PR = (1 << PinNumber);
}
