#include "Sample.h"
#include "AdcDriver.h"


#define AD_CHANNEL_BATVOLT 6
#define AD_CHANNEL_RVCURR  1

#define AD_CHANNEL_BATVOLT_GAIN 0.0088623f  // RealValue = AdValue / 4096 * 3.3 * 110 / 10
#define AD_CHANNEL_RVCURR_GAIN  0.016        // RealValue = (AdValue-2048) * 1/4096 * 3.3 * 20

ADCDATA stAdcData   = {0};
STDCSUM stDcSumTemp = {0};

uint8_t sbSample_Init(void)
{
    return 1;
}

void sSample_IT_Poll(void) //1Ms进入
{
    if(stDcSumTemp.uDcCnt >= 20)
    {
        stAdcData.stBatVolt.fSum = stDcSumTemp.fBatVolt;
        stAdcData.stBatVolt.fAvg = stAdcData.stBatVolt.fSum /   stDcSumTemp.uDcCnt ;
        
        stAdcData.stRvCurr .fSum = stDcSumTemp.fRvCurr;
        stAdcData.stRvCurr.fAvg  = stAdcData.stRvCurr.fSum /   stDcSumTemp.uDcCnt ;
        
        stDcSumTemp.fBatVolt = 0.0f;
        stDcSumTemp.fRvCurr = 0.0f;
        
        stDcSumTemp.uDcCnt   = 0.0f;
    }
    else
    {
        uint16_t AdValue = 0;
        
        /**************电池电流*******************/
        AdValue                        =  adc_channel_sample(AD_CHANNEL_BATVOLT );
        stAdcData.stBatVolt.fRealValue =  AdValue * AD_CHANNEL_BATVOLT_GAIN;
        stDcSumTemp.fBatVolt           += stAdcData.stBatVolt.fRealValue;
        /***************电池电流End***************/
        
        /****************RV 电流采样***************************/
        AdValue                        =  adc_channel_sample(AD_CHANNEL_RVCURR);
        stAdcData.stRvCurr.fRealValue  =  -(AdValue - 2048) * AD_CHANNEL_RVCURR_GAIN;
        stDcSumTemp.fRvCurr            += stAdcData.stRvCurr.fRealValue;
       /****************RV 电流采样End*************************/
        stDcSumTemp.uDcCnt += 1; 
    }
}
