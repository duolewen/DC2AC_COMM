#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#include "gd32f30x.h"
#include "PinDef.h"
#include "stdbool.h"

typedef signed char         S8;
typedef signed short int    S16;
typedef signed int          S32;

typedef uint8_t INT8U;
typedef int8_t  INT8S;

typedef uint16_t INT16U;
typedef int16_t  INT16S;

typedef uint32_t INT32U;
typedef int32_t  INT32S;

typedef float  FLOAT;
typedef double DOUBLE;

typedef bool     BOOL;
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;

typedef int16_t                                 int16;
typedef int32_t                                 int32;
typedef uint8_t                                 Uint8;
typedef uint16_t                                Uint16;
typedef uint32_t                                Uint32;
typedef float                                   float32;
typedef long double                             float64;
typedef uint16_t                                BOOLEAN;



#define E_USE_TESTMODE           0 //测试模式开关
#define E_WATCHDOG_ENABLE        1 //看门狗

#define DSPCOMM_BOUND 19200       //Pcs通信波特率

#define  SILMODE_POWER 150       //静音模式功率设定

#define LENGTH_DSPCOMM_TX  150
#define LENGTH_DSPCOMM_RX  150

#define LENGTH_MODBUS_TX  3000
#define LENGTH_MODBUS_RX  1500

#define DSPCOMM_RXDMA    DMA0
#define DSPCOMM_RXDMA_CH DMA_CH5

#define DSPCOMM_TXDMA    DMA0
#define DSPCOMM_TXDMA_CH DMA_CH6

#define USART2_PRE_PRI    8U
#define USART2_SUB_PRI    0U
 
#define USART1_PRE_PRI    9U
#define USART1_SUB_PRI    0U

#define USART0_PRE_PRI    4U
#define USART0_SUB_PRI    0U



const uint8_t *GetSoftVer(void);
const uint8_t *GetHardtVer(void);
const uint8_t *GetDebugSoftVer(void);
#endif

