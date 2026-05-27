/*********************************************************************
Copyright SOLAX POWER.
File Name  : InitMCU.c
Description: 驱动配置  
Version    :                                                                                                   
Author     :whl                                                    
Date       :                                                            
*********************************************************************/

#include "InitMCU.h"
#include "gd32f30x.h"
#include "GlobalDef.h"
#include "UsartDriver.h"
#include "MyDelay.h"
#include "SpiDriver.h"
#include "TimerDriver.h"
#include "AdcDriver.h"
#include "24cxx.h"

/*********************************************************************
Function name:INT8U sbInitGpioConfig(void)
Description:  
Calls:        
Called By:       
Parameters:   
Return:      
*********************************************************************/
INT8U sbInitGpioConfig(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
    
    //USART0 DEBUG
    gpio_pin_remap_config(GPIO_USART0_REMAP,ENABLE);
    gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_7);
    
    //USART1 BLE
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);
    gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5); //BLE AT
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4); //BLE EN
    
    //USART2 PCS-COMM
    gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
    gpio_init(GPIOC,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_13);
    
    //Simu-I2C EEPROM
    gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_14);
    gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_15);
    /*
    //Simu-I2C USBA
    gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0);
    gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);
    
    //Simu-I2C TYPEC
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
    */
    //AFE-SPI
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_15);//CS
    gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_3); //SCK
    gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_4);//MISO
    gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);//MOSI 
    
    gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);//SHIP
    gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_8);//ALARM
    
    //lCD
    gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_15);//LCD_BLK
    gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_14);//LCD_CS
    gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_13);//LCD_WR
    gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_12);//LCD_DATA
    
    //SWITCH
    gpio_init(GPIOB,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ,GPIO_PIN_2);//Power-SW
    gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_12);//DC-SW
    gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_0); //AC-SW
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_8);      //DC-ONOFF
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);      //USB-ONOFF
    
    //LED
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_7);//LED1
    
    //ADC
    gpio_init(GPIOA,GPIO_MODE_AIN,GPIO_OSPEED_50MHZ,GPIO_PIN_6);//SBAT+
    gpio_init(GPIOA,GPIO_MODE_AIN,GPIO_OSPEED_50MHZ,GPIO_PIN_1);//RV.Current

    AFE_EXIT_SHIPMODE;//退出SHIP模式
    sMy_DelayMs(10);
    
    return 1;
}



/*********************************************************************
Function name: INT8U sbInitMcu(void)
Description:  
Calls:        
Called By:       
Parameters:   
Return:      
*********************************************************************/
INT8U sbInitMcu(void)
{
    INT8U ubTmpRet;

    sbInitGpioConfig();
    
    sbUsartDriver_Init();
    sbSPIDriver_Init();
    sbAdcDriver_Init();
    sbTimer1_Init();
    
  
    AT24CXX_Init();

    
    
//#if(USER_WATCHDOG_ABLE == 1)
//	sIWDG_Init();
//#endif
//
    return ubTmpRet;
}


/*********************************************************************
Function name: 	void sStartMcu(void)
Description:  	
Calls:        	
Called By:       
Parameters:   		
Return:      
*********************************************************************/
void sStartMcu(void)
{
    usart_enable(USART0);
    usart_enable(USART1);
    usart_enable(USART2);
    sUsartDriver_Start();
    
    timer_enable(TIMER1);
}


