#include "SpiDriver.h"
#include "MyDelay.h"
#include "Crc.h"


static void sInitSpi2(void)
{
    spi_parameter_struct spi_struct_parame;
    
    rcu_periph_clock_enable(RCU_SPI2);
    
    spi_struct_parame.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_struct_parame.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_struct_parame.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_struct_parame.device_mode          = SPI_MASTER;
    spi_struct_parame.endian               = SPI_ENDIAN_MSB;
    spi_struct_parame.nss                  = SPI_NSS_SOFT;
    spi_struct_parame.prescale             = SPI_PSC_128;

    spi_init(SPI2, &spi_struct_parame);
    
    spi_enable(SPI2);
}

uint8_t sbSPIDriver_Init(void)
{
    sInitSpi2();
    return 1;
}

uint8_t sbSPI_ReadWrite(uint32_t udwSPI_Device ,uint8_t *pWri,uint8_t *pRead,uint32_t Size)
{
    uint32_t TmpCnt = 0;
    uint32_t Const_Tmp1MS_Cnt = 1000 * 120 / 5 ;
    for(uint32_t i = 0;i < Size;i++)
    {
        TmpCnt = 0;
        while(spi_i2s_flag_get(udwSPI_Device,SPI_FLAG_TBE) == RESET && TmpCnt < Const_Tmp1MS_Cnt)//ÑÓÊ±1Ms
        {
            TmpCnt += 1;
        }
        if(TmpCnt >= Const_Tmp1MS_Cnt)
            return 0;
        
        spi_i2s_data_transmit(udwSPI_Device, pWri[i]);
        
        TmpCnt = 0;
        while(spi_i2s_flag_get(udwSPI_Device,SPI_FLAG_RBNE) == RESET && TmpCnt < Const_Tmp1MS_Cnt)//ÑÓÊ±1Ms
        {
            TmpCnt += 1;
        }
        if(TmpCnt >= Const_Tmp1MS_Cnt)
            return 0;
        
        pRead[i] = spi_i2s_data_receive(udwSPI_Device);
    }
  
    return 1;
}

