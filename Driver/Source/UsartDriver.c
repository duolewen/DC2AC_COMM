#include "UsartDriver.h"
#include "GlobalDef.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "string.h"
#include "InitMCU.h"
#include "semphr.h"

typedef void (*pCallback_USARTRecv)(uint32_t ,uint8_t *,uint32_t);
typedef void (*pCallback_ReInit)(void);

#pragma pack(1)
typedef struct
{
    uint32_t  uwUsart_periph;          /* STM32内部设备串口指针*/
    uint32_t  uwBound;                 /*波特率*/
    uint8_t   *pTxBuf;                 /*发送缓存地址*/
    uint8_t   *pRxBuf;                 /*接收缓存地址*/
    uint16_t  uhwTxBufSize;            /*发送缓存区大小*/
    uint16_t  uhwRxBufSize;            /*接收缓存区大小*/
    uint32_t  uwDmaTx;                 /*发送DMA*/
    dma_channel_enum DmaTx_CH;         /*发送dma通道*/
    uint32_t  uwDmaRx;                 /*接收dma*/
    dma_channel_enum DmaRx_CH;         /*接收dma通道*/
    pCallback_ReInit sCallBackReInit;  /*复位底层回调函数*/ 
    pCallback_USARTRecv sCallBackRecv; /*串口接收到数据后的回调*/
    uint8_t ubFristFlag;               /*首次发送标志位*/
}T_USART;   
#pragma pack()

static uint8_t ubUSART0TXBuff[LENGTH_MODBUS_TX];
static uint8_t ubUSART0RXBuff[LENGTH_MODBUS_RX];


 uint8_t ubUSART2TXBuff[LENGTH_DSPCOMM_TX];
 uint8_t ubUSART2RXBuff[LENGTH_DSPCOMM_RX];

static void sInitUsart0(void);
static void sInitUsart1(void);
static void sInitUsart2(void);


T_USART UsartBuff[] = 
{
    {USART0, 9600,     ubUSART0TXBuff, ubUSART0RXBuff,  LENGTH_MODBUS_TX,          LENGTH_MODBUS_RX,            DMA0,DMA_CH3,DMA0,DMA_CH4,sInitUsart0,0,1},
  
    {USART2, 19200,    ubUSART2TXBuff, ubUSART2RXBuff,  LENGTH_DSPCOMM_TX,         LENGTH_DSPCOMM_RX,           DMA0,DMA_CH1,DMA0,DMA_CH2,sInitUsart2,0,1},
};

static void sResetUsartDma(T_USART *pUsart)
{
    memset(pUsart->pRxBuf,0,pUsart->uhwRxBufSize);
    
    dma_channel_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    dma_transfer_number_config(pUsart->uwDmaRx, pUsart->DmaRx_CH, pUsart->uhwRxBufSize);
    dma_channel_enable(pUsart->uwDmaRx, pUsart->DmaRx_CH);
}


static T_USART* swGetUsartBuff(uint32_t usart_periph)
{
    for(uint32_t i = 0; i < sizeof(UsartBuff) / sizeof(T_USART); i++)
    {
        if(UsartBuff[i].uwUsart_periph == usart_periph){
            return UsartBuff+i;
        }
    }
    return 0;
}


static void sInitUsart0(void)
{
    dma_parameter_struct dma_init_struct;
    
    uint32_t Usartx = USART0;
    T_USART *pUsart = swGetUsartBuff(Usartx);
    
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_DMA0);
    
    usart_deinit(Usartx);
    usart_baudrate_set(Usartx, pUsart->uwBound);
    usart_receive_config(Usartx, USART_RECEIVE_ENABLE);
    usart_transmit_config(Usartx, USART_TRANSMIT_ENABLE);
    
    dma_deinit(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)pUsart->pTxBuf;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = pUsart->uhwTxBufSize;
    dma_init_struct.periph_addr = (INT32U)&USART_DATA(Usartx);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(pUsart->uwDmaTx, pUsart->DmaTx_CH, &dma_init_struct);
    
    dma_circulation_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);   
    
    dma_channel_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    dma_deinit(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)pUsart->pRxBuf;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number =  pUsart->uhwRxBufSize;
    dma_init_struct.periph_addr = (INT32U)&USART_DATA(Usartx);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(pUsart->uwDmaRx, pUsart->DmaRx_CH, &dma_init_struct);
    
    dma_circulation_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH); 
    
    usart_dma_transmit_config(Usartx, USART_TRANSMIT_DMA_ENABLE);
    usart_dma_receive_config(Usartx, USART_RECEIVE_DMA_ENABLE);
    
    nvic_irq_enable(USART0_IRQn, USART0_PRE_PRI, 0);
    
    usart_interrupt_enable(Usartx, USART_INT_IDLE); 
    
    dma_channel_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    dma_channel_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    pUsart->ubFristFlag = 1;
}

static void sInitUsart1(void)
{
    dma_parameter_struct dma_init_struct;
    
    uint32_t Usartx = USART1;
    T_USART *pUsart = swGetUsartBuff(Usartx);
    
    rcu_periph_clock_enable(RCU_USART1);
    rcu_periph_clock_enable(RCU_DMA0);
    
    usart_deinit(Usartx);
    usart_baudrate_set(Usartx, pUsart->uwBound);
    usart_receive_config(Usartx, USART_RECEIVE_ENABLE);
    usart_transmit_config(Usartx, USART_TRANSMIT_ENABLE);
    
    dma_deinit(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)pUsart->pTxBuf;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = pUsart->uhwTxBufSize;
    dma_init_struct.periph_addr = (INT32U)&USART_DATA(Usartx);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(pUsart->uwDmaTx, pUsart->DmaTx_CH, &dma_init_struct);

    dma_circulation_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);   

    dma_channel_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    dma_deinit(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)pUsart->pRxBuf;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number =  pUsart->uhwRxBufSize;
    dma_init_struct.periph_addr = (INT32U)&USART_DATA(Usartx);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(pUsart->uwDmaRx, pUsart->DmaRx_CH, &dma_init_struct);
    
    dma_circulation_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH); 
    
    usart_dma_transmit_config(Usartx, USART_TRANSMIT_DMA_ENABLE);
    usart_dma_receive_config(Usartx, USART_RECEIVE_DMA_ENABLE);
    
    nvic_irq_enable(USART1_IRQn, USART1_PRE_PRI, USART1_SUB_PRI);

    usart_interrupt_enable(Usartx, USART_INT_IDLE); 
    
    dma_channel_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    dma_channel_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);

    pUsart->ubFristFlag = 1;

}

static void sInitUsart2(void)
{
    dma_parameter_struct dma_init_struct;
    
    uint32_t Usartx = USART2;
    T_USART *pUsart = swGetUsartBuff(Usartx);
    
    rcu_periph_clock_enable(RCU_USART2);
    rcu_periph_clock_enable(RCU_DMA0);
    
    usart_deinit(Usartx);
    usart_baudrate_set(Usartx, pUsart->uwBound);
    usart_receive_config(Usartx, USART_RECEIVE_ENABLE);
    usart_transmit_config(Usartx, USART_TRANSMIT_ENABLE);
    
    dma_deinit(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)pUsart->pTxBuf;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = pUsart->uhwTxBufSize;
    dma_init_struct.periph_addr = (INT32U)&USART_DATA(Usartx);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(pUsart->uwDmaTx, pUsart->DmaTx_CH, &dma_init_struct);
    
    dma_circulation_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);   
    
    dma_channel_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    dma_deinit(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)pUsart->pRxBuf;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number =  pUsart->uhwRxBufSize;
    dma_init_struct.periph_addr = (INT32U)&USART_DATA(Usartx);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(pUsart->uwDmaRx, pUsart->DmaRx_CH, &dma_init_struct);
    
    dma_circulation_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH); 
    
    usart_dma_transmit_config(Usartx, USART_TRANSMIT_DMA_ENABLE);
    usart_dma_receive_config(Usartx, USART_RECEIVE_DMA_ENABLE);
    
    nvic_irq_enable(USART2_IRQn, USART2_PRE_PRI, USART2_SUB_PRI);
    
    usart_interrupt_enable(Usartx, USART_INT_IDLE); 
    
    dma_channel_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    dma_channel_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    pUsart->ubFristFlag = 1;
}

uint8_t sbUsartDriver_Init(void)
{
    sInitUsart0();
    sInitUsart1();
    sInitUsart2();
    return 1;
}

void sUsartDriver_Start(void)
{
    T_USART* pTmpUsart0 = swGetUsartBuff(USART0);
    dma_channel_enable(pTmpUsart0->uwDmaRx, pTmpUsart0->DmaRx_CH);
    
    T_USART* pTmpUsart1 = swGetUsartBuff(USART1);
    dma_channel_enable(pTmpUsart1->uwDmaRx, pTmpUsart1->DmaRx_CH);
    
    T_USART* pTmpUsart2 = swGetUsartBuff(USART2);
    dma_channel_enable(pTmpUsart2->uwDmaRx, pTmpUsart2->DmaRx_CH);
}

uint8_t sbUsartDriver_SendData(uint32_t usart_periph,uint8_t *pdata,uint16_t len)
{
    T_USART *pUsart = swGetUsartBuff(usart_periph);
    if(!pUsart)
    {
        return 0;
    }
    if(pUsart->ubFristFlag)//第一次发送 不需要等待标志位
    {
        pUsart->ubFristFlag = 0;
    }
    else //后续发送需要等待标志位
    {
        uint32_t uwXCount = xTaskGetTickCount();
        while(1)
        {
            if(SET == dma_flag_get(pUsart->uwDmaTx,pUsart->DmaTx_CH,DMA_FLAG_FTF) 
                || xTaskGetTickCount() - uwXCount >= 3 * 1000)
            {
                break;
            }
        }
    }
    
    dma_flag_clear(pUsart->uwDmaTx,pUsart->DmaTx_CH,DMA_FLAG_FTF);  
    dma_channel_disable(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    
    memcpy(pUsart->pTxBuf,pdata,len);
    dma_transfer_number_config(pUsart->uwDmaTx, pUsart->DmaTx_CH, (INT32U)len);
    dma_channel_enable(pUsart->uwDmaTx, pUsart->DmaTx_CH);
    
    return 1;
}

void sSetUsartDmaRecvCallback(uint32_t usart_periph ,uint32_t pCallback)
{
    T_USART *pUsart =  swGetUsartBuff(usart_periph);
    pUsart->sCallBackRecv = (pCallback_USARTRecv)pCallback;
}


void sUsartDriver_Reinit(uint32_t usart_periph)
{
    T_USART *pUsart =  swGetUsartBuff(usart_periph);
    if(pUsart->sCallBackReInit)
    {
        pUsart->sCallBackReInit();
    }
}


void USART0_IRQHandler(void)
{
    uint32_t udwTmpUsart = USART0;
    T_USART *pUsart = swGetUsartBuff(udwTmpUsart);
    if(RESET != usart_interrupt_flag_get(udwTmpUsart, (usart_interrupt_flag_enum)USART_INT_FLAG_IDLE))
    {
        usart_flag_get(udwTmpUsart, USART_FLAG_IDLE);
        usart_data_receive(udwTmpUsart);

        dma_channel_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH);

        uint32_t uwLen =  pUsart->uhwRxBufSize - dma_transfer_number_get(pUsart->uwDmaRx, pUsart->DmaRx_CH);
        if(pUsart->sCallBackRecv)
        {
            pUsart->sCallBackRecv(udwTmpUsart,pUsart->pRxBuf,uwLen);
        }
        sResetUsartDma(pUsart);
    }
    else
    {
        sResetUsartDma(pUsart);
    }
}

void USART1_IRQHandler(void)
{
    uint32_t udwTmpUsart = USART1;
    T_USART *pUsart = swGetUsartBuff(udwTmpUsart);
    if(RESET != usart_interrupt_flag_get(udwTmpUsart, (usart_interrupt_flag_enum)USART_INT_FLAG_IDLE))
    {
        usart_flag_get(udwTmpUsart, USART_FLAG_IDLE);
        usart_data_receive(udwTmpUsart);
    
        dma_channel_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH);
    
        uint32_t uwLen =  pUsart->uhwRxBufSize - dma_transfer_number_get(pUsart->uwDmaRx, pUsart->DmaRx_CH);
        if(pUsart->sCallBackRecv)
        {
            pUsart->sCallBackRecv(udwTmpUsart,pUsart->pRxBuf,uwLen);
        }
        sResetUsartDma(pUsart);
    }
    else
    {
        sResetUsartDma(pUsart);
    }
}

void USART2_IRQHandler(void)
{
    uint32_t udwTmpUsart = USART2;
    T_USART *pUsart = swGetUsartBuff(udwTmpUsart);
    if(RESET != usart_interrupt_flag_get(udwTmpUsart, (usart_interrupt_flag_enum)USART_INT_FLAG_IDLE))
    {
        usart_flag_get(udwTmpUsart, USART_FLAG_IDLE);
        usart_data_receive(udwTmpUsart);

        dma_channel_disable(pUsart->uwDmaRx, pUsart->DmaRx_CH);

        uint32_t uwLen =  pUsart->uhwRxBufSize - dma_transfer_number_get(pUsart->uwDmaRx, pUsart->DmaRx_CH);
        if(pUsart->sCallBackRecv)
        {
            pUsart->sCallBackRecv(udwTmpUsart,pUsart->pRxBuf,uwLen);
        }
        sResetUsartDma(pUsart);
    }
    else
    {
        sResetUsartDma(pUsart);
    }
}
