/*
 * timer_driver.h
 *
 *  Created on: Aug 18, 2026
 *      Author: octav
 */

#ifndef DRIVERS_TIMER_DRIVER_H_
#define DRIVERS_TIMER_DRIVER_H_

#include "stm32f411xx.h"

typedef struct {
	uint16_t TIMx_Prescaler;
	uint32_t TIMx_AutoReload;
	uint8_t TIMx_UpdateInterruptEnable;
}TIMx_Config_t;

typedef struct {
    TIMx_Reg_t *pTIMx; // Holds the base addr of the GPIO port to which pin belongs
    TIMx_Config_t TIMx_Config;
}TIMx_Handle_t;

#define TIM_UIE_ENABLE 	1
#define TIM_UIE_DISABLE 	0

#define TIMx_DIER_UIE 0

#define TIMx_SR_UIF 0

#define TIMx_CR1_CEN 0

#define TIM_FLAG_UIF (1U << 0)

/* Control de Reloj del Periférico */
void TIMx_PeriClockControl(TIMx_Reg_t *pTIMx, uint8_t EnorDi);

/* Inicialización y Desinicialización */
void TIMx_Init(TIMx_Handle_t *pTIMHandle);
#endif /* DRIVERS_TIMER_DRIVER_H_ */
