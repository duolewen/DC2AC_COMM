#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "gd32f30x_gpio.h"
#include "gd32f30x_rcu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "globaldef.h"

//SDA IO口宏定义
#define SDA_PROT GPIOB
#define SDA_PIN	 GPIO_PIN_11
#define SDA_RCU	 RCU_GPIOB

//SCL IO口宏定义
#define SCL_PROT GPIOB
#define SCL_PIN  GPIO_PIN_10
#define SCL_RCU  RCU_GPIOB

extern SemaphoreHandle_t Eeprom_semaphore;

INT8U sbCRC8(INT8U *ubpBuf ,INT16U uhwBufSize);
INT8U sbReadData(INT8U *ubpMCUStartAddr,INT8U ubWrLen,INT32U uwEepromSatrtAddr);     
//INT8U sbWriteData(INT8U* ubpMCUStartAddr,INT8U ubWrLen,INT32U uwEepromAddr);

//INT8U sbAT24C08ByteRead(INT32U wWordAddress,INT8U *bpMCUAddress); 
//INT8U sbAT24C08PageRead(INT32U wWordAddress,INT8U *bpMCUAddress,INT16U len); 
INT8U sbAT24C08ByteWrite(INT32U wWordAddress,INT8U *bpMCUAddress); 
INT8U sbAT24C08PageWrite(INT32U wWordAddress,INT8U *bpMCUAddress,INT16U len);
#endif
