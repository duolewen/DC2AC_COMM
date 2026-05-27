/*******************************************************************************
*
*  $Id: PinUse.h   2011-10-20   weir.guo $
*
*  Define Pin use
*  PinDef.h
*******************************************************************************/

#ifndef PINDEF_H_
#define PINDEF_H_

#include "gd32f30x_gpio.h"

/*************************BLE************************************/
#define BLE_POWER_ON        gpio_bit_reset(GPIOA,GPIO_PIN_4)
#define BLE_POWER_OFF       gpio_bit_set(GPIOA,GPIO_PIN_4)
#define BLE_AT_ON           gpio_bit_set(GPIOA,GPIO_PIN_5)
#define BLE_AT_OFF          gpio_bit_reset(GPIOA,GPIO_PIN_5)
/********************* BLE END************************************/

/***********************AFE*****************************************/
#define GPIO_SPI_CSEN       gpio_bit_reset(GPIOA,GPIO_PIN_15)
#define GPIO_SPI_CSDis      gpio_bit_set(GPIOA,GPIO_PIN_15)

#define AFE_ALARM_STATUS      gpio_input_bit_get(GPIOB,GPIO_PIN_8)
#define AFE_ENTER_SHIPMODE    gpio_bit_reset(GPIOB,GPIO_PIN_9)
#define AFE_EXIT_SHIPMODE     gpio_bit_set(GPIOB,GPIO_PIN_9)
/***********************AFE END*************************************/

/************************LCD****************************************/
#define CS_LOW    gpio_bit_reset(GPIOB,GPIO_PIN_14)
#define CS_HIGH   gpio_bit_set(GPIOB,GPIO_PIN_14)

#define WR_LOW    gpio_bit_reset(GPIOB,GPIO_PIN_13)
#define WR_HIGH   gpio_bit_set(GPIOB,GPIO_PIN_13)
#define WR_OUT    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13)

#define DA_LOW    gpio_bit_reset(GPIOB,GPIO_PIN_12)
#define DA_HIGH   gpio_bit_set(GPIOB,GPIO_PIN_12)
#define DA_OUT    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12)

#define LCD_POW_ON    gpio_bit_set(GPIOB,GPIO_PIN_15)
#define LCD_POW_OFF   gpio_bit_reset(GPIOB,GPIO_PIN_15)
/********************LCD END********************************/

/**********************SIMU I2C**************************************/
#define EEP_SCL_GPIOx      GPIOC
#define EEP_SCL_GPIO_PINx  GPIO_PIN_14
#define EEP_SDA_GPIOx      GPIOC
#define EEP_SDA_GPIO_PINx  GPIO_PIN_15

#define USBA_SCL_GPIOx      GPIOB
#define USBA_SCL_GPIO_PINx  GPIO_PIN_0
#define USBA_SDA_GPIOx      GPIOB
#define USBA_SDA_GPIO_PINx  GPIO_PIN_1

#define TYPEC_SCL_GPIOx      GPIOA
#define TYPEC_SCL_GPIO_PINx  GPIO_PIN_11
#define TYPEC_SDA_GPIOx      GPIOA
#define TYPEC_SDA_GPIO_PINx  GPIO_PIN_10
/**********************SIMU I2C END**********************************/

/*************************SWITCH*************************************/
#define POWER_DET_GPIO        GPIOB
#define POWER_DET_GPIO_PIN    GPIO_PIN_2

#define AC_DET_GPIO       GPIOA
#define AC_DET_GPIO_PIN   GPIO_PIN_0

#define DC_DET_GPIO       GPIOA
#define DC_DET_GPIO_PIN   GPIO_PIN_12

#define DC_SW_ON          gpio_bit_reset(GPIOA,GPIO_PIN_8)
#define DC_SW_OFF         gpio_bit_set(GPIOA,GPIO_PIN_8)

#define USB_SW_ON         gpio_bit_set(GPIOA,GPIO_PIN_9)
#define USB_SW_OFF        gpio_bit_reset(GPIOA,GPIO_PIN_9)

/***********************SWITCH END***********************************/

//PCS
#define PCS_MONITOR_GPIO       GPIOC
#define PCS_MONITOR_GPIO_PIN   GPIO_PIN_13

//LED
#define LED1_ON              gpio_bit_set(GPIOA,GPIO_PIN_7)
#define LED1_OFF             gpio_bit_reset(GPIOA,GPIO_PIN_7)
#define LED1_ONOFF_STATUS    gpio_output_bit_get(GPIOA,GPIO_PIN_7)

#endif /*PINDEF_H_*/
