#ifndef _SIMULATIONLICDRIVER_H_
#define _SIMULATIONLICDRIVER_H_

#include "gd32f30x.h"
#include <stdio.h>
#include "stdint.h"

typedef struct
{
    uint32_t SDA_PROT;
    uint32_t SDA_PIN;
    uint32_t SCL_PROT;
    uint32_t SCL_PIN;
    uint32_t usClk;
}T_SIMU_I2C_DESC;

void IIC_Init(T_SIMU_I2C_DESC *pI2c);
void IIC_Start(T_SIMU_I2C_DESC *pI2c);
void IIC_Stop(T_SIMU_I2C_DESC *pI2c);
uint8_t IIC_Wait_Ack(T_SIMU_I2C_DESC *pI2c);
void IIC_Ack(T_SIMU_I2C_DESC *pI2c);
void IIC_NAck(T_SIMU_I2C_DESC *pI2c);
void IIC_Send_Byte(T_SIMU_I2C_DESC *pI2c,uint8_t txd);
uint8_t IIC_Read_Byte(T_SIMU_I2C_DESC *pI2c,uint8_t ack);

#endif
