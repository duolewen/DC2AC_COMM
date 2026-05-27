#ifndef HAL_USART_H_
#define HAL_USART_H_

#include "GlobalDef.h"


uint8_t sbUsartDriver_Init(void);
uint8_t sbUsartDriver_SendData(uint32_t usart_periph,uint8_t *pdata,uint16_t len);
void sUsartDriver_Reinit(uint32_t usart_periph);
void sUsartDriver_Start(void);
void sSetUsartDmaRecvCallback(uint32_t usart_periph ,uint32_t pCallback);

#endif

