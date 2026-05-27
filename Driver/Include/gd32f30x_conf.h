/**
  ******************************************************************************
  * @file    GPIO/IOToggle/gd32f30x_conf.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Library configuration file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F30x_CONF_H
#define __GD32F30x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
#include "gd32f30x_adc.h"
#include "gd32f30x_bkp.h"
//#include "gd32f30x_can.h"
//#include "gd32f30x_cec.h"
//#include "gd32f30x_crc.h"
//#include "gd32f30x_dac.h"
//#include "gd32f30x_dbgmcu.h"
#include "gd32f30x_dma.h"
//#include "gd32f30x_exti.h"
#include "gd32f30x_fmc.h"
//#include "gd32f30x_fsmc.h"
#include "gd32f30x_gpio.h"
//#include "gd32f30x_i2c.h"
#include "gd32f30x_fwdgt.h"
#include "gd32f30x_pmu.h"
#include "gd32f30x_rcu.h"
#include "gd32f30x_rtc.h"
//#include "gd32f30x_sdio.h"
#include "gd32f30x_spi.h"
#include "gd32f30x_timer.h"
#include "gd32f30x_usart.h"
//#include "gd32f30x_wwdg.h"
#include "gd32f30x_misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __GD32F30x_CONF_H*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
