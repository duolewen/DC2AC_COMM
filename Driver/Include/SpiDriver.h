#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include "stdint.h"
#include "GlobalDef.h"

uint8_t sbSPIDriver_Init(void);
uint8_t sbSPI_ReadWrite(uint32_t udwSPI_Device ,uint8_t *pWri,uint8_t *pRead,uint32_t Size);

#endif
