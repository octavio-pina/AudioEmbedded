/*
 * i2s_driver.h
 *
 *  Created on: Sep 17, 2026
 *      Author: octav
 */

 #ifndef DRIVERS_I2S_DRIVER_H_
#define DRIVERS_I2S_DRIVER_H_

#include "stm32f411xx.h"
#include "spi_driver.h"
/*
 *  Configuration structure for I2Sx peripheral
 */
typedef struct
{
    uint8_t I2S_ConfigMode;
    uint8_t I2S_Standard;
    uint8_t I2S_ClockPolarity;
    uint8_t I2S_DataLen;
    uint8_t I2S_ChannelLen;
    uint8_t I2S_SampleRate;
    uint8_t I2S_MCLK_Enable;

} I2S_Config_t;

/*
 *Handle structure for I2Sx peripheral
 */
typedef struct
{
    SPI_RegDef_t *pI2Sx; /*!< This holds the base address of I2Sx(x:0,1,2) peripheral >*/
    I2S_Config_t I2SConfig;
} I2S_Handle_t;

/*
 *Handle structure for PLL I2S Param
 */
typedef struct
{
    uint16_t PLLI2SM;
    uint16_t PLLI2SR;
    uint16_t PLLI2SN;
} PLLI2S_Handle_t;

#define I2S_PLL_M     0
#define I2S_PLL_N     6
#define I2S_PLL_R    28

/*
 * @I2S_ConfigMode
 */
#define I2S_CFG_SLAVE_TX 0
#define I2S_CFG_SLAVE_RX 1
#define I2S_CFG_MASTER_TX 2
#define I2S_CFG_MASTER_RX 3

/*
 * @I2S_Standard
 */
#define I2S_STD_PHILIPS 0
#define I2S_STD_MSBJUST 1
#define I2S_STD_LSBJUST 2
#define I2S_STD_PCM 3

/*
 * @I2S_ClockPolarity
 */
#define I2S_CKPOL_LOW 0
#define I2S_CKPOL_HIGH 1

/*
 * @I2S_DataLen
 */
#define I2S_DATALEN_16BITS 0
#define I2S_DATALEN_24BITS 1
#define I2S_DATALEN_32BITS 2

/*
 * @I2S_ChannelLen
 */
#define I2S_CHLEN_16BITS 0
#define I2S_CHLEN_32BITS 1

/*
 * @I2S_SampleRate
 */
#define I2S_SAMPLE_RATE_44K1 0
#define I2S_SAMPLE_RATE_48K 1
#define I2S_SAMPLE_RATE_96K 2

/*
 * @I2S_MCLK_Enable
 */
#define I2S_MCKOE_DISABLE 0
#define I2S_MCKOE_ENABLE 1


// Bit position definition of I2S Peripheral
// SPI_I2SCFGR
#define I2SCFGR_I2SMOD  11
#define I2SCFGR_I2SE    10
#define I2SCFGR_I2SCFG  8
#define I2SCFGR_PCMSYNC 7
#define I2SCFGR_I2SSTD  4
#define I2SCFGR_CKPOL   3
#define I2SCFGR_DATLEN  1
#define I2SCFGR_CHLEN   0

// SPI_I2SPR
#define I2SPR_MCKOE   9
#define I2SPR_ODD   8
#define I2SPR_I2SDIV   0

//FLAGS
#define I2S_FLAG_TXE (1U << SPI_SR_TXE)

/******************************************************************************************
 *								APIs supported
 * by this driver For more information about the APIs check the function
 * definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void I2S_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void I2S_PLLConfig(PLLI2S_Handle_t *pPLLI2SHandler);

/*
 * Init and De-init
 */
void I2S_Init(I2S_Handle_t *pI2SHandle);
#endif /* DRIVERS_I2S_DRIVER_H_ */