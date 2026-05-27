#include "MyDelay.h"

void sMy_DelayUs(uint32_t udwUs)
{
    for(uint32_t i = 0; i < udwUs; i++)
    {
        for(uint32_t j = 0; j < 120/5; j++)
        {
            __nop();
        }
    }
}

void sMy_DelayMs(uint32_t udwMs)
{
    for(uint32_t i = 0; i < udwMs; i++)
    {
        sMy_DelayUs(1000);
    }
}
