/*********************************************************************
Copyright SOLAX POWER.
File Name  : 24cxx.c
Description: 存储芯片驱动 
Version    :                                                                                                   
Author     :whl                                                    
Date       :                                                            
*********************************************************************/
#include "24cxx.h"
#include "SimulationIicDriver.h"
#include "MyDelay.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "string.h"

T_SIMU_I2C_DESC tEeI2c = {
    . SDA_PROT = EEP_SDA_GPIOx,
    . SDA_PIN  = EEP_SDA_GPIO_PINx,
    . SCL_PROT = EEP_SCL_GPIOx,
    . SCL_PIN  = EEP_SCL_GPIO_PINx,
    . usClk    = 11,
};

SemaphoreHandle_t xMutex_24cxx    = 0;

uint8_t eeprom_text_buffer[] = "write and read eeprom test";
//测试片外eeprom读写是否正常.正常返回true

/*********************************************************************
Function name: void AT24CXX_Init(void)
Description: 初始化IIC接口     
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
void AT24CXX_Init(void)
{
    //创建互斥量
     xMutex_24cxx = xSemaphoreCreateMutex();
     if (xMutex_24cxx == NULL)
     {
        // 创建互斥量失败的处理
     }
    
    IIC_Init(&tEeI2c);//IIC初始化
}

/*********************************************************************
Function name: INT8U AT24CXX_ReadOneByte(INT16U ReadAddr)
Description: 在AT24CXX指定地址读出一个数据     
Calls:            
Called By:       
Parameters: ReadAddr:开始读数的地址          
Return: 读到的数据                  
*********************************************************************/
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp   = 0;
    uint8_t h_add  = 0;
    uint8_t l_add  = 0;

    IIC_Start(&tEeI2c);
    
    
    h_add = (uint8_t)(((ReadAddr >> 8) & 0x0f) << 1);
    l_add = (uint8_t)(ReadAddr  & 0xff);
    
    IIC_Send_Byte(&tEeI2c,0XA0 | h_add); //发送器件地址0XA0,写数据
    
    IIC_Wait_Ack(&tEeI2c);
    IIC_Send_Byte(&tEeI2c,l_add); //发送低地址
    IIC_Wait_Ack(&tEeI2c);
    IIC_Start(&tEeI2c);
    IIC_Send_Byte(&tEeI2c,0XA1 | h_add); //进入接收模式
    IIC_Wait_Ack(&tEeI2c);
    temp = IIC_Read_Byte(&tEeI2c,0);
    IIC_NAck(&tEeI2c);//whl 20230912
    IIC_Stop(&tEeI2c);//产生一个停止条件
    
    return temp;
}

/*********************************************************************
Function name: void AT24CXX_WriteOneByte(INT16U WriteAddr,INT8U DataToWrite)
Description:  在AT24CXX指定地址写入一个数据    
Calls:            
Called By:       
Parameters:   WriteAddr :写入数据的目的地址        DataToWrite:要写入的数据
Return:                   
*********************************************************************/
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
    uint8_t h_add  = 0;
    uint8_t l_add  = 0;
    
    IIC_Start(&tEeI2c);
    
    h_add = (uint8_t)(((WriteAddr >> 8) & 0x0f) << 1);
    l_add = (uint8_t)(WriteAddr  & 0xff);
    
    IIC_Send_Byte(&tEeI2c,0XA0 + h_add); //发送器件地址0XA0,写数据
    
    IIC_Wait_Ack(&tEeI2c);
    IIC_Send_Byte(&tEeI2c,l_add); //发送低地址
    IIC_Wait_Ack(&tEeI2c);
    IIC_Send_Byte(&tEeI2c,DataToWrite); //发送字节
    IIC_Wait_Ack(&tEeI2c);
    IIC_Stop(&tEeI2c);//产生一个停止条件
}


/*********************************************************************
Function name: void AT24CXX_WriteLenByte(INT16U WriteAddr,INT32U DataToWrite,INT8U Len)
Description: 在AT24CXX里面的指定地址开始写入长度为Len的数据     该函数用于写入16bit或者32bit的数据.
Calls:            
Called By:       
Parameters:  WriteAddr :开始写入的地址     DataToWrite:数据数组首地址    Len :要写入数据的长度2,4
Return:                   
*********************************************************************/
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{
    uint8_t t = 0;
    
    for (t=0;t<Len;t++)
    {
        AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
    }
}


/*********************************************************************
Function name: INT32U AT24CXX_ReadLenByte(INT16U ReadAddr,INT8U Len)
Description: 在AT24CXX里面的指定地址开始读出长度为Len的数据 该函数用于读出16bit或者32bit的数据.    
Calls:            
Called By:       
Parameters: ReadAddr :开始读出的地址      Len :要读出数据的长度2,4    
Return: 数据                  
*********************************************************************/
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{
    uint8_t t     = 0;
    uint32_t temp = 0;

    for (t=0;t<Len;t++)
    {
        temp <<= 8;
        temp +=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
    }
    
    return temp;
}

/*********************************************************************
Function name: INT8U AT24CXX_Check(void)
Description: 检查AT24CXX是否正常 这里用了24XX的最后一个地址(0)来存储标志字.    如果用其他24C系列,这个地址要修改
Calls:            
Called By:       
Parameters:           
Return:     1:检测失败         0:检测成功     
*********************************************************************/
uint8_t AT24CXX_Check(void)
{
    uint8_t temp;

    temp = AT24CXX_ReadOneByte(0);//避免每次开机都写AT24CXX
    
    if (temp == 0X55)
    {    
        return 1;
    }
    else//排除第一次初始化的情况
    {
        AT24CXX_WriteOneByte(0,0X55);
        
        temp=AT24CXX_ReadOneByte(0);
        
        if (temp == 0X55)
        {
            return 1;
        }
    }
    
    return 0;
}



/*********************************************************************
Function name: void AT24CXX_Read(INT16U ReadAddr,INT8U *pBuffer,INT16U NumToRead)
Description: 在AT24CXX里面的指定地址开始读出指定个数的数据
Calls:            
Called By:       
Parameters:ReadAddr :开始读出的地址 对24c02为0~255 pBuffer :数据数组首地址  NumToRead:要读出数据的个数
Return:                   
*********************************************************************/

void AT24CXX_Read_All(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
    uint8_t h_add  = 0;
    uint8_t l_add  = 0;

    xSemaphoreTake(xMutex_24cxx, portMAX_DELAY) ;
   
    IIC_Start(&tEeI2c);
    h_add = (uint8_t)(((ReadAddr >> 8) & 0x0f) << 1);
    l_add = (uint8_t)(ReadAddr  & 0xff);    
    IIC_Send_Byte(&tEeI2c,0XA0 | h_add); //发送器件地址0XA0,写数据    
    IIC_Wait_Ack(&tEeI2c);
    IIC_Send_Byte(&tEeI2c,l_add); //发送低地址
    IIC_Wait_Ack(&tEeI2c);
    IIC_Start(&tEeI2c);
    IIC_Send_Byte(&tEeI2c,0XA1 | h_add); //进入接收模式
    IIC_Wait_Ack(&tEeI2c);

    while (NumToRead)
    {
        *pBuffer++ = IIC_Read_Byte(&tEeI2c,0);
        NumToRead--;
        if(NumToRead !=0)
        {
            IIC_Ack(&tEeI2c);
        }
    }
    IIC_NAck(&tEeI2c);  
    IIC_Stop(&tEeI2c); //产生一个停止条件

    xSemaphoreGive(xMutex_24cxx) ;

}


void AT24CXX_Write_All(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
    uint16_t WriteAddrTemp = WriteAddr;
    uint16_t Writelen = 0;
    uint8_t h_add  = 0;
    uint8_t l_add  = 0;


    xSemaphoreTake(xMutex_24cxx, portMAX_DELAY) ;
    
    while (NumToWrite != 0)
    {
        Writelen = PAGE_SIZE - WriteAddrTemp % PAGE_SIZE;
        IIC_Start(&tEeI2c);
        h_add = (uint8_t)(((WriteAddrTemp >> 8) & 0x0f) << 1);
        l_add = (uint8_t)(WriteAddrTemp  & 0xff);
        IIC_Send_Byte(&tEeI2c,0XA0 + h_add); //发送器件地址0XA0,写数据
        IIC_Wait_Ack(&tEeI2c);
        IIC_Send_Byte(&tEeI2c,l_add); //发送低地址
        IIC_Wait_Ack(&tEeI2c);
        while (Writelen != 0 && NumToWrite != 0)
        {
            
            IIC_Send_Byte(&tEeI2c,*pBuffer); //发送字节
            IIC_Wait_Ack(&tEeI2c);
            pBuffer++;
            WriteAddrTemp++;
            NumToWrite--;
            Writelen--;
        }
        IIC_Stop(&tEeI2c);//产生一个停止条件
        sMy_DelayMs(15);    
    }
    xSemaphoreGive(xMutex_24cxx) ;
    
}


/*******************************************************************************
  * 函数名：x24Cxx_WritePage
  * 功  能：页写
  * 参  数：u16Addr要写入的首地址；
            u8Len写入数据字节数，最大为PAGE_SIZE
            pData要写入的数据首地址
  * 返回值：无
  * 说  明：最多写入1页，防止翻卷，如果地址跨页则去掉跨页的部分
*******************************************************************************/
void x24Cxx_WriteByte(uint16_t u16Addr, uint8_t u8Data)
{
    IIC_Start(&tEeI2c);//起始信号
    IIC_Send_Byte(&tEeI2c,DEV_ADDR | WRITE_CMD | (((uint8_t)((u16Addr >> 8) & 0x07)) << 1));//器件寻址+写+页选择位
    IIC_Wait_Ack(&tEeI2c);//等待应答
    IIC_Send_Byte(&tEeI2c,(uint8_t)(u16Addr & 0xFF));//只取地址的低字节，高字节如果有，已经按照页选择位处理过了
    IIC_Wait_Ack(&tEeI2c);//等待应答
    IIC_Send_Byte(&tEeI2c,u8Data);
    IIC_Wait_Ack(&tEeI2c);//等待应答
    IIC_Stop(&tEeI2c);//停止信号
}

/*******************************************************************************
  * 函数名：x24Cxx_WritePage
  * 功  能：页写
  * 参  数：u16Addr要写入的首地址；
            u8Len写入数据字节数，最大为PAGE_SIZE
            pData要写入的数据首地址
  * 返回值：无
  * 说  明：最多写入1页，防止翻卷，如果地址跨页则去掉跨页的部分
*******************************************************************************/
uint8_t x24Cxx_ReadByte(uint16_t u16Addr)
{
    uint8_t u8Data = 0;
    
    IIC_Start(&tEeI2c);//起始信号    
    IIC_Send_Byte(&tEeI2c,DEV_ADDR | WRITE_CMD | (((uint8_t)((u16Addr >> 8) & 0x07)) << 1));//器件寻址+写+页选择位
    IIC_Wait_Ack(&tEeI2c);//等待应答
    IIC_Send_Byte(&tEeI2c,(uint8_t)(u16Addr & 0xFF));//只取地址的低字节，高字节如果有，已经按照页选择位处理过了
    IIC_Wait_Ack(&tEeI2c);//等待应答
    IIC_Start(&tEeI2c);//起始信号
    IIC_Send_Byte(&tEeI2c,DEV_ADDR | READ_CMD);//器件寻址+读
    IIC_Wait_Ack(&tEeI2c);//等待应答
    u8Data = IIC_Read_Byte(&tEeI2c,0);
    IIC_NAck(&tEeI2c);
    IIC_Stop(&tEeI2c);//停止信号
    return u8Data;
}





/*********************************************************************
Function name: INT8U eeprom_test_func(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
uint8_t eeprom_test_func(void)
{
    uint8_t ret = 0,datatemp[30] ={0};
    
    AT24CXX_Write_All(EEPROM_TEST_FUNC_ADDR,eeprom_text_buffer,sizeof(eeprom_text_buffer));
    
    AT24CXX_Read_All(EEPROM_TEST_FUNC_ADDR,(uint8_t*)datatemp,sizeof(eeprom_text_buffer));
    
    if (0 == memcmp(datatemp,eeprom_text_buffer,sizeof(eeprom_text_buffer)))
    {
        ret = 1;
    }

    return ret;
}

void  AT24Cxx_RW_DisAble(void)
{
    xSemaphoreTake(xMutex_24cxx, portMAX_DELAY) ;
}
