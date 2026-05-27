/*********************************************************************
Copyright SOLAX POWER.
File Name  : FWDGT.c
Description: Î¹¹·Çý¶¯ 
Version    : V1.0                                                                                                   
Author     :                                                     
Date       :                                                            
*********************************************************************/

#include "FWDGT.h"
#include "string.h"
#include "GlobalDef.h"

uint16_t fwdgt_FlagBuff[E_WDGT_FLAG_MAX] = {0};

void sIWDG_Feed(uint8_t Flag)
{
   fwdgt_FlagBuff[Flag]  = 1;

   if(E_WDGT_FLAG_MODPROBLETASK != Flag)
         return ;
   
   for(uint32_t i = 0;i < E_WDGT_FLAG_MAX ;i++)
   {
       if(fwdgt_FlagBuff[i] != 1)
       {
           return ;
       }
   }
   memset(fwdgt_FlagBuff,0,sizeof(fwdgt_FlagBuff));
   
#if (USER_WATCHDOG_ABLE == 1)
   fwdgt_counter_reload();
#endif
}


void sIWDG_Init(void)
{
    /* configure FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
    fwdgt_config(1250, FWDGT_PSC_DIV256);

    /* after 8 seconds to generate a reset */
    fwdgt_enable();
}

