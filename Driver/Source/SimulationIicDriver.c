/*********************************************************************
Copyright SOLAX POWER.
File Name  : SimulationIicDriver.c
Description: 模拟IIC 驱动 
Version    : V1.0                                                                                                   
Author     :                                                     
Date       :                                                            
*********************************************************************/
#include "SimulationIicDriver.h"
#include "MyDelay.h"

/*********************************************************************
Function name: static void IIC_SCL_1(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static void IIC_SCL_1(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    GPIO_BOP(pI2c->SCL_PROT) = pI2c->SCL_PIN;
}


/*********************************************************************
Function name: static void IIC_SCL_0(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static void IIC_SCL_0(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    GPIO_BC(pI2c->SCL_PROT) = pI2c->SCL_PIN;
}


/*********************************************************************
Function name: static void IIC_SDA_1(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static void IIC_SDA_1(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    gpio_bit_set(pI2c->SDA_PROT,pI2c->SDA_PIN);
}


/*********************************************************************
Function name:static void IIC_SDA_0(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static void IIC_SDA_0(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    gpio_bit_reset(pI2c->SDA_PROT,pI2c->SDA_PIN);
}


/*********************************************************************
Function name: static void SDA_IN(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static void SDA_IN(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    gpio_init(pI2c->SDA_PROT , GPIO_MODE_IPU , GPIO_OSPEED_50MHZ , pI2c->SDA_PIN);
}

/*********************************************************************
Function name:static void SDA_OUT(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static void SDA_OUT(T_SIMU_I2C_DESC *pI2c)
{ 
    if(!pI2c)
        return ;
    gpio_init(pI2c->SDA_PROT , GPIO_MODE_OUT_PP , GPIO_OSPEED_50MHZ , pI2c->SDA_PIN);
}

/*********************************************************************
Function name: static INT8U iic_sda_read(void)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static uint8_t iic_sda_read(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return 0;
    if (RESET == gpio_input_bit_get(pI2c->SDA_PROT, pI2c->SDA_PIN))
    {
        return 0x00;
    }
    else
    {
        return 0x01;
    }
}

/*********************************************************************
Function name: static void iic_sda_write(INT8U bit)
Description:      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
static void iic_sda_write(T_SIMU_I2C_DESC *pI2c,uint8_t bit)
{
    if(!pI2c)
        return ;
    if (bit == 0)
    {
        GPIO_BC(pI2c->SDA_PROT) = pI2c->SDA_PIN;
    }
    else
    {
        GPIO_BOP(pI2c->SDA_PROT) = pI2c->SDA_PIN;
    }
}

/*********************************************************************
Function name: void IIC_Init(void)
Description:初始化IIC      
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
void IIC_Init(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    SDA_OUT(pI2c); //输出
    IIC_SCL_1(pI2c);
    sMy_DelayUs(pI2c->usClk);
    IIC_SDA_1(pI2c);
}

/*********************************************************************
Function name: void IIC_Start(void)
Description:产生IIC起始信号
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
void IIC_Start(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    SDA_OUT(pI2c); //输出
    IIC_SDA_1(pI2c);
    IIC_SCL_1(pI2c);
    sMy_DelayUs(4);
    IIC_SDA_0(pI2c);
    sMy_DelayUs(pI2c->usClk);
    IIC_SCL_0(pI2c);
}

/*********************************************************************
Function name: void IIC_Stop(void)
Description:产生IIC停止信号
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
void IIC_Stop(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    SDA_OUT(pI2c);//sda线输出
    IIC_SDA_0(pI2c);//发送I2C总线结束信号
    IIC_SCL_0(pI2c);
    sMy_DelayUs(5);
  
    IIC_SCL_1(pI2c);
    sMy_DelayUs(pI2c->usClk);

    IIC_SDA_1(pI2c);//发送I2C总线结束信号
    sMy_DelayUs(5);
}


/*********************************************************************
Function name: INT8U IIC_Wait_Ack(void)
Description:等待应答信号到来 
Calls:            
Called By:       
Parameters:           
Return:     1，接收应答失败      0，接收应答成功           
*********************************************************************/
uint8_t IIC_Wait_Ack(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return 0;
    uint8_t ucErrTime=0; 
    SDA_IN(pI2c); //SDA设置为输入
    sMy_DelayUs(pI2c->usClk);
    IIC_SCL_1(pI2c);//时钟输出0
    while (iic_sda_read(pI2c))
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_SCL_0(pI2c);//时钟输出0
            return 0;
        }
    }
    IIC_SCL_0(pI2c);//时钟输出0
    return 1;
}


/*********************************************************************
Function name: void IIC_Ack(void)
Description:产生ACK应答
Calls:            
Called By:       
Parameters:           
Return: 
*********************************************************************/
void IIC_Ack(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    SDA_OUT(pI2c);//SDA设置为输入
    IIC_SDA_0(pI2c);
    IIC_SCL_1(pI2c);
    sMy_DelayUs(1);
    IIC_SCL_0(pI2c);
    IIC_SDA_1(pI2c);
}

/*********************************************************************
Function name: void IIC_NAck(void)
Description:不产生ACK应答
Calls:            
Called By:       
Parameters:           
Return:      
*********************************************************************/
void IIC_NAck(T_SIMU_I2C_DESC *pI2c)
{
    if(!pI2c)
        return ;
    
    IIC_SCL_0(pI2c);
    SDA_OUT(pI2c);
    IIC_SDA_1(pI2c);
    sMy_DelayUs(5);
    IIC_SCL_1(pI2c);
    sMy_DelayUs(pI2c->usClk);
    IIC_SCL_0(pI2c);
}


/*********************************************************************
Function name: void IIC_Send_Byte(INT8U txd)
Description:IIC发送一个字节  返回从机有无应答 
Calls:            
Called By:       
Parameters:           
Return:      1，有应答      0，无应答           
*********************************************************************/
void IIC_Send_Byte(T_SIMU_I2C_DESC *pI2c,uint8_t txd)
{
    uint8_t t;
    if(!pI2c)
        return ;

    SDA_OUT(pI2c);//SDA设置为输入
    IIC_SCL_0(pI2c);//拉低时钟开始数据传输    
    
    for (t=0;t<8;t++)
    {
        iic_sda_write(pI2c,(txd&0x80)>>7);
        txd<<=1;
        sMy_DelayUs(pI2c->usClk); //对TEA5767这三个延时都是必须的
        IIC_SCL_1(pI2c);
        sMy_DelayUs(pI2c->usClk);
        IIC_SCL_0(pI2c);
    }
}


/*********************************************************************
Function name: INT8U IIC_Read_Byte(INT8U ack)
Description:读1个字节，ack=1时，发送ACK，ack=0，发送nACK
Calls:            
Called By:       
Parameters:           
Return:                   
*********************************************************************/
uint8_t IIC_Read_Byte(T_SIMU_I2C_DESC *pI2c,uint8_t ack)
{
    uint8_t i,receive=0;
    if(!pI2c)
        return 0;
    SDA_IN(pI2c);//SDA设置为输入
    for (i=0;i<8;i++ )
    {
        IIC_SCL_0(pI2c);        
        sMy_DelayUs(pI2c->usClk);        
        IIC_SCL_1(pI2c);        
        sMy_DelayUs(pI2c->usClk);
        receive <<= 1;        
        if (iic_sda_read(pI2c))
        {
            receive++;
        }
    }
    IIC_SCL_0(pI2c);
    return receive;
}
