/*
 * spi_driver.h
 *
 *  Created on: Aug 18, 2026
 *      Author: octav
 */

#ifndef DRIVERS_SPI_DRIVER_H_
#define DRIVERS_SPI_DRIVER_H_

#include "stm32f411xx.h"

/*
 *  Configuration structure for SPIx peripheral
 */
typedef struct
{
    uint8_t SPI_DeviceMode;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_FirstBit;
    uint8_t SPI_DFF;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
} SPI_Config_t;

/*
 *Handle structure for SPIx peripheral
 */
typedef struct
{
    SPI_RegDef_t *pSPIx; /*!< This holds the base address of SPIx(x:0,1,2) peripheral >*/
    SPI_Config_t SPIConfig;
} SPI_Handle_t;

/*
 * SPI application states
 */
#define SPI_READY      0
#define SPI_BUSY_IN_RX 1
#define SPI_BUSY_IN_TX 2

/*
 * Possible SPI Application events
 */
#define SPI_EVENT_TX_CMPLT 1
#define SPI_EVENT_RX_CMPLT 2
#define SPI_EVENT_OVR_ERR  3
#define SPI_EVENT_CRC_ERR  4

/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE  0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD             1
#define SPI_BUS_CONFIG_HD             2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY 3

/*
 * @SPI_SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2   0
#define SPI_SCLK_SPEED_DIV4   1
#define SPI_SCLK_SPEED_DIV8   2
#define SPI_SCLK_SPEED_DIV16  3
#define SPI_SCLK_SPEED_DIV32  4
#define SPI_SCLK_SPEED_DIV64  5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7

/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS  0
#define SPI_DFF_16BITS 1

/*
 * @CPOL
 */
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW  0

/*
 * @CPHA
 */
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW  0
/*
 * @CPHA
 */
#define SPI_LSBFIRST 	1
#define SPI_MSBFIRST  	0

/*
 * @SPI_SSM
 */
#define SPI_SSM_EN 1
#define SPI_SSM_DI 0

// Bit position definition of SPI Peripheral
// SPI_CR1
#define SPI_CR1_CPHA     0
#define SPI_CR1_CPOL     1
#define SPI_CR1_MSTR     2
#define SPI_CR1_BR       3
#define SPI_CR1_SPE      6
#define SPI_CR1_LSBFIRST 7
#define SPI_CR1_SSI      8
#define SPI_CR1_SSM      9
#define SPI_CR1_RXONLY   10
#define SPI_CR1_DFF      11
#define SPI_CR1_CRCNEXT  12
#define SPI_CR1_CRCEN    13
#define SPI_CR1_BIDIOE   14
#define SPI_CR1_BIDIMODE 15

// SPI_CR2
#define SPI_CR2_RXDMAEN 0
#define SPI_CR2_TXDMAEN 1
#define SPI_CR2_SSOE    2
#define SPI_CR2_FRF     4
#define SPI_CR2_ERRIE   5
#define SPI_CR2_RXNEIE  6
#define SPI_CR2_TXEIE   7

// SPI_SR
#define SPI_SR_RXNE   0
#define SPI_SR_TXE    1
#define SPI_SR_CHSIDE 2
#define SPI_SR_UDR    3
#define SPI_SR_CRCERR 4
#define SPI_SR_MODF   5
#define SPI_SR_OVR    6
#define SPI_SR_BSY    7
#define SPI_SR_FRE    8



#define SPI_MAX_WAIT 10000U
/******************************************************************************************
 *								APIs supported
 * by this driver For more information about the APIs check the function
 * definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);

/*
 * Data Send and Receive
 */

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

uint8_t SPI_TransferByte(SPI_RegDef_t *pSPIx, uint8_t txByte);
bool SPI_ReceiveBuffer(SPI_RegDef_t *pSPIx, uint8_t *buffer, uint16_t length);
#endif /* DRIVERS_SPI_DRIVER_H_ */
