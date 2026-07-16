#include "gd32f30x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include "24cxx.h"

#include "FLASH.h"
#include "InitMCU.h"

#include "FWDGT.h"
#include "DSP_M3_COMTask.h"


static TaskHandle_t InitTaskHandler = NULL;

static TaskHandle_t LcdTaskHandler = NULL;
enum
{
    E_INIT_TASK_PRIO =  tskIDLE_PRIORITY + 1 ,
  
    E_LCD_TASK_PRIO,
 
};

const static uint8_t ubDEBUG_SOFTVER[4] = {0,0,0,10};

void init_task(void * pvParameters);

int  main(void)
{ 
    /* configure 4 bits pre-emption priority */
    __enable_irq();
    //nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x6000);//
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x00);//
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    
    /* init task */
    xTaskCreate(init_task, "INIT", configMINIMAL_STACK_SIZE* 2, NULL, E_INIT_TASK_PRIO, &InitTaskHandler);
    
    /* start scheduler */
    vTaskStartScheduler();

    while(1)
    {
        ;
    }
}    


void sLcdTask(void * pvParameters)
{
    while(1)
    {
        vTaskDelay(1000);
        sbUsartDriver_SendData(USART2,"1234",4);
    }
}

void init_task(void * pvParameters)
{    
    uint8_t ret = 1;
   
    /*******GPIO初始化及外设驱动初始化*********/
    ret &= sbInitMcu();
    /*******GPIO初始化及外设驱动初始化 END*****/
    
    /*******Eeprom相关初始化******************/
    GDFLASH_Init();

    /*******Eeprom相关初始化 END*************/
    
    /*************显示相关初始化***************************/
  
    /*************显示相关初始化END***************************/
    
    /*******Modbus相关初始化******************/
    
    /*******Modbus相关初始化 END**************/
    
    //Pcs通信相关初始化
    //sbModDspComm_Init();
    //sbAppDspCommClient_Init();
    
    /*************BMS初始化****************** */
   
    /*************BMS初始化 END***************/
    
    //USBA-Typec相关初始化
    
    /******蓝牙相关初始化******************/

    /******蓝牙相关初始化END***************/
    
    sStartMcu();

    xTaskCreate(sDSP_M3ComTxTask,          "sDSP_M3ComTxTask",      configMINIMAL_STACK_SIZE * 4, NULL,      E_LCD_TASK_PRIO,           &LcdTaskHandler);
    xTaskCreate(sDSP_M3ComRxTask,          "sDSP_M3ComRxTask",      configMINIMAL_STACK_SIZE * 4, NULL,      E_LCD_TASK_PRIO,           &LcdTaskHandler);
   
    
    xTaskCreate(sShowTask,          "sShowTask",      configMINIMAL_STACK_SIZE * 4, NULL,      E_LCD_TASK_PRIO,           &LcdTaskHandler);
  

    if(!ret)
    {
        vTaskDelete(LcdTaskHandler);
        while(1)
        {
           ;
        }
    }
    for( ;; )
    {
        vTaskDelete(NULL);
    }
}

const uint8_t *GetSoftVer(void)
{
    return ubDEBUG_SOFTVER;
}

