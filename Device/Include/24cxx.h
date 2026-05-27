/*********************************************************************
Copyright SOLAX POWER.
File Name  : 24cxx.h
Description: This file shows a standard source file format
Version    : V1.0                                                                                                   
Author     :                                                     
Date       :                                                            
*********************************************************************/
#ifndef __24CXX_H
#define __24CXX_H
#include "GlobalDef.h"

#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C128 16383
#define AT24C256 32767

#define MB85RC16 2046


//使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C08


#define READ_CMD        1
#define WRITE_CMD       0

#define x24C08                      //器件名称，x24C04、x24C08或x24C16
#define DEV_ADDR        0xA0        //设备硬件地址

#ifdef x24C04
     #define PAGE_NUM            32                        //页数
    #define PAGE_SIZE            16                        //页面大小(字节)
    #define CAPACITY_SIZE        (PAGE_NUM * PAGE_SIZE)    //总容量(字节)
    #define ADDR_BYTE_NUM        1                        //地址字节个数
#endif
 
#ifdef x24C08
     #define PAGE_NUM            64                        //页数
    #define PAGE_SIZE            16                        //页面大小(字节)
    #define CAPACITY_SIZE        (PAGE_NUM * PAGE_SIZE)    //总容量(字节)
    #define ADDR_BYTE_NUM        1                        //地址字节个数
#endif
 
#ifdef x24C16
     #define PAGE_NUM            128                        //页数
    #define PAGE_SIZE            16                        //页面大小(字节)
    #define CAPACITY_SIZE        (PAGE_NUM * PAGE_SIZE)    //总容量(字节)
    #define ADDR_BYTE_NUM        1                        //地址字节个数
#endif


//-------------------eeprom存储区的规划使用
//0~1用来存储检测设备标志AT24CXX_Check()
#define EEPROM_TEST_FUNC_ADDR 0 //测试eeprom读写时的位置数据区

//-------------------


/*********************************************************************
Function name: void AT24CXX_Init(void)
Description: 初始化IIC接口     
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
void AT24CXX_Init(void);

/*********************************************************************
Function name: INT8U AT24CXX_ReadOneByte(INT16U ReadAddr)
Description: 在AT24CXX指定地址读出一个数据     
Calls:            
Called By:       
Parameters: ReadAddr:开始读数的地址          
Return: 读到的数据                  
*********************************************************************/
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);

/*********************************************************************
Function name: void AT24CXX_WriteOneByte(INT16U WriteAddr,INT8U DataToWrite)
Description:  在AT24CXX指定地址写入一个数据    
Calls:            
Called By:       
Parameters:   WriteAddr :写入数据的目的地址        DataToWrite:要写入的数据
Return:                   
*********************************************************************/
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);

/*********************************************************************
Function name: void AT24CXX_WriteLenByte(INT16U WriteAddr,INT32U DataToWrite,INT8U Len)
Description: 在AT24CXX里面的指定地址开始写入长度为Len的数据     该函数用于写入16bit或者32bit的数据.
Calls:            
Called By:       
Parameters:  WriteAddr :开始写入的地址     DataToWrite:数据数组首地址    Len :要写入数据的长度2,4
Return:                   
*********************************************************************/
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);

/*********************************************************************
Function name: INT32U AT24CXX_ReadLenByte(INT16U ReadAddr,INT8U Len)
Description: 在AT24CXX里面的指定地址开始读出长度为Len的数据 该函数用于读出16bit或者32bit的数据.    
Calls:            
Called By:       
Parameters: ReadAddr :开始读出的地址      Len :要读出数据的长度2,4    
Return: 数据                  
*********************************************************************/
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);


/*********************************************************************
Function name: INT8U AT24CXX_Check(void)
Description: 检查AT24CXX是否正常 这里用了24XX的最后一个地址(0)来存储标志字.    如果用其他24C系列,这个地址要修改
Calls:            
Called By:       
Parameters:           
Return:     1:检测失败         0:检测成功     
*********************************************************************/
uint8_t AT24CXX_Check(void);

/*********************************************************************
Function name: void AT24CXX_Read(INT16U ReadAddr,INT8U *pBuffer,INT16U NumToRead)
Description: 在AT24CXX里面的指定地址开始读出指定个数的数据
Calls:            
Called By:       
Parameters:ReadAddr :开始读出的地址 对24c02为0~255 pBuffer :数据数组首地址  NumToRead:要读出数据的个数
Return:                   
*********************************************************************/

void AT24CXX_Read_All(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);

/*********************************************************************
Function name: void AT24CXX_Write(INT16U WriteAddr,INT8U *pBuffer,INT16U NumToWrite)
Description: 在AT24CXX里面的指定地址开始写入指定个数的数据     
Calls:            
Called By:       
Parameters: WriteAddr :开始写入的地址 对24c02为0~255  pBuffer :数据数组首地址    NumToWrite:要写入数据的个数    
Return:                   
*********************************************************************/

void AT24CXX_Write_All(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);

/*********************************************************************
Function name: INT8U eeprom_test_func(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
uint8_t eeprom_test_func(void);

void  AT24Cxx_RW_DisAble(void);
#endif













