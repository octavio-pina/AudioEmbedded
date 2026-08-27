/*
 * uart_driver.h
 *
 *  Created on: Aug 17, 2026
 *      Author: octav
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "stm32f411xx.h"

typedef struct {
	uint8_t USARTx_Mode;
	uint32_t USARTx_BaudRate;
	uint8_t USARTx_ParityCtrl;
	uint8_t USARTx_StopBits;
	uint8_t USARTx_WordLength;
	uint8_t USARTx_RXInterruptEnable;
}USART_PinConfig_t;

typedef struct {
    USARTx_Reg_t *pUSARTx; // Holds the base addr of the GPIO port to which pin belongs
    USART_PinConfig_t USART_PinConfig;
}USART_Handle_t;

/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX    2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_9600   9600
#define USART_STD_BAUD_19200  19200
#define USART_STD_BAUD_38400  38400
#define USART_STD_BAUD_57600  57600
#define USART_STD_BAUD_115200 115200

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1   0
#define USART_STOPBITS_0_5 1
#define USART_STOPBITS_2   2
#define USART_STOPBITS_1_5 3

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS 0
#define USART_WORDLEN_9BITS 1

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD  2
#define USART_PARITY_EN_EVEN 1
#define USART_PARITY_DISABLE 0

#define USART_IRQ_DISABLE 0
#define USART_IRQ_ENABLE 1

//USART FLAG MASK
#define USART_FLAG_RXNE (1U << 5)
#define USART_FLAG_ORE (1U << 3)

/******************************************************************************************
 *Bit position definitions of USART peripheral
 ******************************************************************************************/
/*
 * Bit position definitions USART_SR
 */
#define USART_SR_PE   0
#define USART_SR_FE   1
#define USART_SR_NF   2
#define USART_SR_ORE  3
#define USART_SR_IDLE 4
#define USART_SR_RXNE 5
#define USART_SR_TC   6
#define USART_SR_TXE  7
#define USART_SR_LBD  8
#define USART_SR_CTS  9

/*
 * Bit position definitions USART_DR
 */
#define USART_DR 0

/*
 * Bit position definitions USART_BRR
 */
#define USART_BRR_FRACTION 0
#define USART_BRR_MANTISSA 4

/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK    0
#define USART_CR1_RWU    1
#define USART_CR1_RE     2
#define USART_CR1_TE     3
#define USART_CR1_IDLEIE 4
#define USART_CR1_RXNEIE 5
#define USART_CR1_TCIE   6
#define USART_CR1_TXEIE  7
#define USART_CR1_PEIE   8
#define USART_CR1_PS     9
#define USART_CR1_PCE    10
#define USART_CR1_WAKE   11
#define USART_CR1_M      12
#define USART_CR1_UE     13
#define USART_CR1_OVER8  15

/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD    0
#define USART_CR2_LBDL   5
#define USART_CR2_LBDLIE 6
#define USART_CR2_LBCL   8
#define USART_CR2_CPHA   9
#define USART_CR2_CPOL   10
#define USART_CR2_CKLEN  11
#define USART_CR2_STOP   12
#define USART_CR2_LINEN  14

/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE    0
#define USART_CR3_IREN   1
#define USART_CR3_IRLP   2
#define USART_CR3_HDSEL  3
#define USART_CR3_NACK   4
#define USART_CR3_SCEN   5
#define USART_CR3_DMAR   6
#define USART_CR3_DMAT   7
#define USART_CR3_RTSE   8
#define USART_CR3_CTSE   9
#define USART_CR3_CTSIE  10
#define USART_CR3_ONEBIT 11

/*
 * ============================================================================
 * APIs Soportadas por este Driver
 * ============================================================================
 */

void USART_PeriClockControl(USARTx_Reg_t *pUSARTx, uint8_t EnorDi);

/* Inicialización y Desinicialización */
void USART_Init(USART_Handle_t *pUSARTHandle);

/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTx, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQHandling(USART_Handle_t *pHandle);

#endif /* UART_DRIVER_H_ */
