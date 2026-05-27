#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include "GlobalDef.h"

uint8_t  sbAdcDriver_Init(void);
uint16_t adc_channel_sample(uint8_t channel);

#endif
