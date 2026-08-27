/*
 * stm32f411xx.h
 *
 *  Created on: Aug 16, 2026
 *      Author: octav
 */

#ifndef STM32F411XX_H_
#define STM32F411XX_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SYSTICK_CLOCK 16000000ULL

#define __vo   volatile
#define __weak __attribute__((weak))

// ARM CORTEX Mx Processor NVIC ISERx Register Address
#define NVIC_ISER0 ((__vo uint32_t *)0xE000E100)
#define NVIC_ISER1 ((__vo uint32_t *)0xE000E104)
#define NVIC_ISER2 ((__vo uint32_t *)0xE000E108)
#define NVIC_ISER3 ((__vo uint32_t *)0xE000E10C)

// ARM CORTEX Mx Processor NVIC ICERx Register Addr
#define NVIC_ICER0 ((__vo uint32_t *)0XE000E180)
#define NVIC_ICER1 ((__vo uint32_t *)0XE000E184)
#define NVIC_ICER2 ((__vo uint32_t *)0XE000E188)
#define NVIC_ICER3 ((__vo uint32_t *)0XE000E18C)

// ARM CORTEX Mx Processor NVIC Priority Register Addr
#define NVIC_IPR_ADDR ((__vo uint32_t *)0xE000E400)

#define FLASH_BASEADDR 0x08000000U /* Base Address of Flash Memory */
#define SRAM_BASEADDR  0x20000000U /* Base Address of SRAM Memory */
#define ROM_BASEADDR   0x1FFF0000U /* Base Address of System Memory */

// AHBx and APBx Bus Peripheral base Address

#define PERP_ADDR 0x40000000U
#define APB1_ADDR PERP_ADDR
#define APB2_ADDR 0x40010000U
#define AHB1_ADDR 0x40020000U
#define AHB2_ADDR 0x50000000U

// Base Addr of peripherals which are hanging on AHB1 bus

#define GPIOA_ADDR (AHB1_ADDR + 0x0000)
#define GPIOB_ADDR (AHB1_ADDR + 0x0400)
#define GPIOC_ADDR (AHB1_ADDR + 0x0800)
#define GPIOD_ADDR (AHB1_ADDR + 0x0C00)
#define GPIOE_ADDR (AHB1_ADDR + 0x1000)
#define GPIOH_ADDR (AHB1_ADDR + 0x1C00)
#define RCC_ADDR   (AHB1_ADDR + 0x3800)

// Base Addr of peripherals which are hanging on APB1 bus
#define TIM5_ADDR 	(APB1_ADDR + 0x0C00)
#define TIM4_ADDR 	(APB1_ADDR + 0x0800)
#define TIM3_ADDR 	(APB1_ADDR + 0x0400)
#define TIM2_ADDR 	(APB1_ADDR + 0x0000)

#define SPI3_ADDR (APB1_ADDR + 0x3C00)
#define SPI2_ADDR (APB1_ADDR + 0x3800)

#define USART2_ADDR (APB1_ADDR + 0x4400)
// Base Addr of peripherals which are hanging on APB2 bus

#define SPI1_ADDR (APB2_ADDR + 0x3000)
#define SPI4_ADDR (APB2_ADDR + 0x3400)
#define SPI5_ADDR (APB2_ADDR + 0x5000)

#define TIM11_ADDR 	(APB2_ADDR + 0x4800)
#define TIM10_ADDR 	(APB2_ADDR + 0x4400)
#define TIM9_ADDR 		(APB2_ADDR + 0x4000)
#define TIM1_ADDR 		(APB2_ADDR + 0x0000)

#define USART1_ADDR 	(APB2_ADDR + 0x1400)
#define USART6_ADDR 	(APB2_ADDR + 0x1000)

#define EXTI_ADDR 		(APB2_ADDR + 0x3C00)
#define SYSCFG_ADDR 	(APB2_ADDR + 0x3800)

// Peripheral Register Definition Structures
// GPIO
typedef struct
{
	__vo uint32_t MODER;   // GPIO port mode register
	__vo uint32_t OTYPER;  // GPIO port output type register
	__vo uint32_t OSPEEDR; // GPIO port output speed register
	__vo uint32_t PUPDR;   // GPIO port pull-up/pull-down register
	__vo uint32_t IDR;     // GPIO port input data register
	__vo uint32_t ODR;     // GPIO port output data register
	__vo uint32_t BSRR;    // GPIO port bit set/reset register
	__vo uint32_t LCKR;    // GPIO port configuration lock register
	__vo uint32_t AFR[2];  // GPIO alternate function low[0] & high[1] register
} GPIOx_Reg_t;

// SPI
typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;
} SPI_RegDef_t;

// USART
typedef struct
{
    __vo uint32_t SR;
    __vo uint32_t DR;
    __vo uint32_t BRR;
    __vo uint32_t CR1;
    __vo uint32_t CR2;
    __vo uint32_t CR3;
    __vo uint32_t GTPR;
} USARTx_Reg_t;

// TIMx
typedef struct
{
    __vo uint32_t CR1;      // 0x00
    __vo uint32_t CR2;      // 0x04
    __vo uint32_t SMCR;     // 0x08
    __vo uint32_t DIER;     // 0x0C
    __vo uint32_t SR;       // 0x10
    __vo uint32_t EGR;      // 0x14
    __vo uint32_t CCMR1;    // 0x18
    __vo uint32_t RESERVED; // 0x1C
    __vo uint32_t CCER;     // 0x20
    __vo uint32_t CNT;      // 0x24
    __vo uint32_t PSC;      // 0x28
    __vo uint32_t ARR;      // 0x2C
    __vo uint32_t RESERVED1;// 0x30
    __vo uint32_t CCR1;     // 0x34
    __vo uint32_t CCR2;     // 0x38
} TIMx_Reg_t;

//RCC
typedef struct
{
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t RESERVED0[2];
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	__vo uint32_t RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t RESERVED2[2];
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	__vo uint32_t RESERVED3[2];
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t RESERVED4[2];
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	__vo uint32_t RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	__vo uint32_t RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
	__vo uint32_t RESERVED7;
	__vo uint32_t DCKCFGR;
} RCC_Reg_t;
// EXTI
typedef struct
{
	__vo uint32_t IMR;		//Interrupt Mask
	__vo uint32_t EMR;		//Event Mask
	__vo uint32_t RTSR;		//Rising Trigger Selection
	__vo uint32_t FTSR;		//Falling Trigger Selection
	__vo uint32_t SWIER;	//SW Interrupt Event
	__vo uint32_t PR;		//Pending
} EXTI_Reg_t;
// SYSCFG
typedef struct
{
	__vo uint32_t MEMRMP; 		//Memory Remap
	__vo uint32_t PMC;			//Peripheral Mode Config
	__vo uint32_t EXTICR[4];	//External Interrupt Config
	__vo uint32_t RESERVED[2];
	__vo uint32_t CMPCR;		//Compensation cell control
} SYSCFG_Reg_t;
// Peripheral Def AHB1

#define GPIOA 	((GPIOx_Reg_t *)GPIOA_ADDR)
#define GPIOB 	((GPIOx_Reg_t *)GPIOB_ADDR)
#define GPIOC 	((GPIOx_Reg_t *)GPIOC_ADDR)
#define GPIOD 	((GPIOx_Reg_t *)GPIOD_ADDR)
#define GPIOE 	((GPIOx_Reg_t *)GPIOE_ADDR)
#define GPIOH 	((GPIOx_Reg_t *)GPIOH_ADDR)

#define RCC 	((RCC_Reg_t *)RCC_ADDR)

#define SPI1 ((SPI_RegDef_t *)SPI1_ADDR)
#define SPI2 ((SPI_RegDef_t *)SPI2_ADDR)
#define SPI3 ((SPI_RegDef_t *)SPI3_ADDR)
#define SPI4 ((SPI_RegDef_t *)SPI4_ADDR)
#define SPI5 ((SPI_RegDef_t *)SPI5_ADDR)

// Peripheral Def APB1
#define USART2 ((USARTx_Reg_t *)USART2_ADDR)
#define TIM2 	((TIMx_Reg_t *)TIM2_ADDR)
#define TIM3 	((TIMx_Reg_t *)TIM3_ADDR)
#define TIM4 	((TIMx_Reg_t *)TIM4_ADDR)
#define TIM5 	((TIMx_Reg_t *)TIM5_ADDR)

// Peripheral Def APB2
#define TIM11 	((TIMx_Reg_t *)TIM11_ADDR)
#define TIM10 	((TIMx_Reg_t *)TIM10_ADDR)
#define TIM9 	((TIMx_Reg_t *)TIM9_ADDR)
#define TIM1 	((TIMx_Reg_t *)TIM1_ADDR)
#define USART1 ((USARTx_Reg_t *)USART1_ADDR)
#define USART6 ((USARTx_Reg_t *)USART6_ADDR)
#define EXTI 	((EXTI_Reg_t *)EXTI_ADDR)
#define SYSCFG ((SYSCFG_Reg_t *)SYSCFG_ADDR)

//CLK EN Macros for GPIOx Peripherals
#define PA_PCLK_EN() (RCC->AHB1ENR |= (1 << 0))
#define PB_PCLK_EN() (RCC->AHB1ENR |= (1 << 1))
#define PC_PCLK_EN() (RCC->AHB1ENR |= (1 << 2))
#define PD_PCLK_EN() (RCC->AHB1ENR |= (1 << 3))
#define PE_PCLK_EN() (RCC->AHB1ENR |= (1 << 4))
#define PH_PCLK_EN() (RCC->AHB1ENR |= (1 << 7))

//CLK EN Macros for USARTx Peripherals
#define USART1_PCLK_EN() (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN() (RCC->APB1ENR |= (1 << 17))
#define USART6_PCLK_EN() (RCC->APB2ENR |= (1 << 5))

//CLK EN Macros for SYSCFG Peripheral
#define SYSCFG_PCKL_EN() (RCC->APB2ENR |= (1 << 14))

// IRQ (Interrupt Request) Numbers of STM32F411RETx MCU

#define IRQ_NO_EXTI0     6
#define IRQ_NO_EXTI1     7
#define IRQ_NO_EXTI2     8
#define IRQ_NO_EXTI3     9
#define IRQ_NO_EXTI4     10
#define IRQ_NO_EXTI9_5   23
#define IRQ_NO_EXTI15_10 40

#define IRQ_NO_TIM1_BRK_TIM9		24
#define IRQ_NO_TIM1_UP_TIM10		25
#define IRQ_NO_TIM1_TRG_COM_TIM11	26
#define IRQ_NO_TIM1_CC				27
#define IRQ_NO_TIM2				28
#define IRQ_NO_TIM3				29
#define IRQ_NO_TIM4				30
#define IRQ_NO_TIM5				50

#define IRQ_NO_SPI1 35
#define IRQ_NO_SPI2 36
#define IRQ_NO_SPI3 51
#define IRQ_NO_SPI4 84
#define IRQ_NO_SPI5 85

#define IRQ_NO_I2C1_EV 31
#define IRQ_NO_I2C1_ER 32
#define IRQ_NO_I2C2_EV 33
#define IRQ_NO_I2C2_ER 34
#define IRQ_NO_I2C3_EV 72
#define IRQ_NO_I2C3_ER 73

#define IRQ_NO_USART1 37
#define IRQ_NO_USART2 38
#define IRQ_NO_USART6 71
/*
 * macros for all the possible priority levels
 */
#define NVIC_IRQ_PRI0  0
#define NVIC_IRQ_PRI15 15

// Generics

#define ENABLE     1
#define DISABLE    0
#define HIGH       0
#define LOW        1
#define SET        ENABLE
#define RESET      DISABLE
#define FLAG_RESET RESET
#define FLAG_SET   SET

void NVIC_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void NVIC_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

#endif /* STM32F411XX_H_ */
