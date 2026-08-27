/*
 * gpio_driver.h
 *
 *  Created on: Aug 16, 2026
 *      Author: octav
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "stm32f411xx.h"

typedef struct
{
    uint8_t GPIO_PinNum;        /*!< Possible Values for @GPIO_PIN_NUM >*/
    uint8_t GPIO_PinMode;       /*!< Possible Values for @GPIO_PIN_MODES >*/
    uint8_t GPIO_PinSpeed;      /*!< Possible Values for @GPIO_PIN_SPEEDS >*/
    uint8_t GPIO_PinPuPdCtlr;   /*!< Possible Values for @GPIO_PIN_PUPD >*/
    uint8_t GPIO_PinOPType;     /*!< Possible Values for @GPIO_PIN_OPT >*/
    uint8_t GPIO_PinAltFunMode; /*!< Possible Values for @GPIO_PIN_MODES >*/
} GPIO_PinConfig_t;

typedef struct
{
    GPIOx_Reg_t *pGPIOx; // Holds the base addr of the GPIO port to which pin belongs
    GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

/*
 * @GPIO_PIN_NUM
 * GPIO Pin Possible Numbers
 * */
#define GPIO_PIN0  0
#define GPIO_PIN1  1
#define GPIO_PIN2  2
#define GPIO_PIN3  3
#define GPIO_PIN4  4
#define GPIO_PIN5  5
#define GPIO_PIN6  6
#define GPIO_PIN7  7
#define GPIO_PIN8  8
#define GPIO_PIN9  9
#define GPIO_PIN10 10
#define GPIO_PIN11 11
#define GPIO_PIN12 12
#define GPIO_PIN13 13
#define GPIO_PIN14 14
#define GPIO_PIN15 15

/*
 * @GPIO_PIN_MODES
 * GPIO Pin Possible Modes
 * */
#define GPIO_MODE_IN     0
#define GPIO_MODE_OUT    1
#define GPIO_MODE_ALTFUN 2
#define GPIO_MODE_ANALOG 3

#define GPIO_MODE_IT_FT  4  // Falling Trigger
#define GPIO_MODE_IT_RT  5  // Rising Trigger
#define GPIO_MODE_IT_RFT 6  // Rising-Falling Trigger

/*
 * @GPIO_PIN_OPT
 * GPIO pin possible output types
 * */
#define GPIO_OPT_PP 0
#define GPIO_OPT_OD 1

/*
 * @GPIO_PIN_SPEEDS
 * GPIO Pin Possible Speeds
 * */
#define GPIO_SPEED_LOW  0
#define GPIO_SPEED_MED  1
#define GPIO_SPEED_FST  2
#define GPIO_SPEED_VFST 3

/*
 * @GPIO_PIN_PUPD
 * GPIO pin Pull up & Pull Down configuration macros
 * */
#define GPIO_NO_PUPD 0
#define GPIO_PIN_PU  1
#define GPIO_PIN_PD  2

/*
 * @GPIO_PIN_ALTFUN
 * GPIO Pin Alternate Function Selection
 */
#define GPIO_AF0   0
#define GPIO_AF1   1
#define GPIO_AF2   2
#define GPIO_AF3   3
#define GPIO_AF4   4   // Ej: I2C1
#define GPIO_AF5   5   // Ej: SPI1/SPI2/I2S2
#define GPIO_AF6   6   // Ej: SPI3/I2S3
#define GPIO_AF7   7   // Ej: USART1/USART2
#define GPIO_AF8   8
#define GPIO_AF9   9
#define GPIO_AF10  10
#define GPIO_AF11  11
#define GPIO_AF12  12
#define GPIO_AF13  13
#define GPIO_AF14  14
#define GPIO_AF15  15

#define GPIO_PIN_HIGH 	1
#define GPIO_PIN_LOW 	0

/*
 * ============================================================================
 * APIs Soportadas por este Driver
 * ============================================================================
 */

/* Control de Reloj del Periférico */
void GPIO_PeriClockControl(GPIOx_Reg_t *pGPIOx, uint8_t EnorDi);

/* Inicialización y Desinicialización */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIOx_Reg_t *pGPIOx);

/* Lectura y Escritura de Datos */
uint8_t GPIO_ReadFromInputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIOx_Reg_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIOx_Reg_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber);

/* Manejo de Interrupciones (EXTI / NVIC) */
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* GPIO_DRIVER_H_ */
