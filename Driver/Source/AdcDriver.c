#include "AdcDriver.h"
#include "MyDelay.h"

uint8_t sbAdcDriver_Init(void)
{
    rcu_periph_clock_enable(RCU_ADC0);
    
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1U);
    
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL, ADC0_1_2_EXTTRIG_ROUTINE_NONE); 
    /* ADC external trigger config */
    adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    
    sMy_DelayMs(5U);
    
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    return 1;
}

uint16_t adc_channel_sample(uint8_t channel)
{
    /* ADC regular channel config */
    adc_routine_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_7POINT5);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_routine_data_read(ADC0));
}


