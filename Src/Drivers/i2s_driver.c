
#include "i2s_driver.h"

void I2S_PeriClockControl(SPI_RegDef_t *pI2Sx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pI2Sx == SPI1) RCC->APB2ENR |= (1U << 12);
		else if(pI2Sx == SPI2) RCC->APB1ENR |= (1U << 14);
		else if(pI2Sx == SPI3) RCC->APB1ENR |= (1U << 15);
		else if(pI2Sx == SPI4) RCC->APB2ENR |= (1U << 13);
		else if(pI2Sx == SPI5) RCC->APB2ENR |= (1U << 20);
	}
	//TODO DISABLE
}

void I2S_PLLConfig(PLLI2S_Handle_t *pPLLI2SHandler){
    RCC->CR &= ~(1u << 26);
    while(RCC->CR & (1u << 27)); //RDY = 0
    RCC->PLLI2SCFGR &= ~(0x7u << I2S_PLL_R); 
    RCC->PLLI2SCFGR |= (pPLLI2SHandler->PLLI2SR << I2S_PLL_R); 
    RCC->PLLI2SCFGR &= ~(0x1FFu << I2S_PLL_N);
    RCC->PLLI2SCFGR |= (pPLLI2SHandler->PLLI2SN << I2S_PLL_N);
    RCC->PLLI2SCFGR &= ~(0x3Fu << I2S_PLL_M);
    RCC->PLLI2SCFGR |= (pPLLI2SHandler->PLLI2SM << I2S_PLL_M);
    RCC->CR |= (1u << 26);
    while(!(RCC->CR & (1u << 27))); //RDY = 1
}

void I2S_Init(I2S_Handle_t *pI2SHandle){
    I2S_PeriClockControl(pI2SHandle->pI2Sx, ENABLE);
    //Ensure the I2S Peripheral is DISABLE
    pI2SHandle->pI2Sx->I2SCFGR &= ~(1U << I2SCFGR_I2SE);

    pI2SHandle->pI2Sx->I2SCFGR |= (1U << I2SCFGR_I2SMOD);
    //Mode
    pI2SHandle->pI2Sx->I2SCFGR &= ~(3U << I2SCFGR_I2SCFG); 
    pI2SHandle->pI2Sx->I2SCFGR |= (pI2SHandle->I2SConfig.I2S_ConfigMode << I2SCFGR_I2SCFG);

    // STD
    pI2SHandle->pI2Sx->I2SCFGR &= ~(3U << I2SCFGR_I2SSTD); //I2S_STD_PHILIPS
    pI2SHandle->pI2Sx->I2SCFGR |= (pI2SHandle->I2SConfig.I2S_Standard << I2SCFGR_I2SSTD);

    //CKPOL
    if(pI2SHandle->I2SConfig.I2S_ClockPolarity == I2S_CKPOL_LOW){
        pI2SHandle->pI2Sx->I2SCFGR &= ~(1U << I2SCFGR_CKPOL);
    }
    else {
        pI2SHandle->pI2Sx->I2SCFGR |= (1U << I2SCFGR_CKPOL);
    }

    //DATLEN
    pI2SHandle->pI2Sx->I2SCFGR &= ~(3U << I2SCFGR_DATLEN);
    pI2SHandle->pI2Sx->I2SCFGR &= ~(1U << I2SCFGR_CHLEN);
    if(pI2SHandle->I2SConfig.I2S_DataLen == I2S_DATALEN_16BITS){
        pI2SHandle->pI2Sx->I2SCFGR |= (pI2SHandle->I2SConfig.I2S_ChannelLen << I2SCFGR_CHLEN);
    } else if (pI2SHandle->I2SConfig.I2S_DataLen <= I2S_DATALEN_32BITS ){
        pI2SHandle->pI2Sx->I2SCFGR |= (pI2SHandle->I2SConfig.I2S_DataLen << I2SCFGR_DATLEN);
        pI2SHandle->pI2Sx->I2SCFGR |= (I2S_CHLEN_32BITS << I2SCFGR_CHLEN); //FIXED 32 BITS
    }

    //MCKOE
    if(pI2SHandle->I2SConfig.I2S_MCLK_Enable == I2S_MCKOE_DISABLE){
        pI2SHandle->pI2Sx->I2SPR &= ~(1U << I2SPR_MCKOE);
    }
    else {
        pI2SHandle->pI2Sx->I2SPR |= (1U << I2SPR_MCKOE);
    }

    //I2SDIV
    if(pI2SHandle->I2SConfig.I2S_SampleRate == I2S_SAMPLE_RATE_48K){
        pI2SHandle->pI2Sx->I2SPR &= ~(1U << I2SPR_ODD);
        pI2SHandle->pI2Sx->I2SPR &= ~(0xFFU << I2SPR_I2SDIV);
        pI2SHandle->pI2Sx->I2SPR |= (0x1F << I2SPR_I2SDIV);
    }

    //Enable the I2S Peripheral
    pI2SHandle->pI2Sx->I2SCFGR |= (1U << I2SCFGR_I2SE);
}