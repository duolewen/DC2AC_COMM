#ifndef SAMPLE_H_
#define SAMPLE_H_

#include "GlobalDef.h"

#pragma pack(1)
typedef struct
{
    float32 fBatVolt;  //  电池电流      
    float32 fRvCurr;   //  RV电流
    //其他模拟量可以在这里添加
    
    Uint16  uDcCnt;              
}STDCSUM;
#pragma pack()

#pragma pack(1)
typedef struct
{ 
    float32 fRealValue;//瞬时实际值
    float32 fSum      ;//代数和
    float32 fAvg      ;//平均值
} ADCCALC_DC;
#pragma pack()

#pragma pack(1)
typedef struct
{
    ADCCALC_DC stBatVolt;   //电池电压
    ADCCALC_DC stRvCurr;    //RV电流采样
}ADCDATA;
#pragma pack()

extern ADCDATA stAdcData;

uint8_t sbSample_Init(void);
void sSample_IT_Poll(void) ; //1Ms进入

#endif
