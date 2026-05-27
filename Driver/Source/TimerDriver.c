#include "TimerDriver.h"
#include "Led.h"

uint8_t sbTimer1_Init(void)
{
    timer_parameter_struct timer_initpara;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER1);

    /* deinit a TIMER */
    timer_deinit(TIMER1);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 120 - 1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 1000 - 1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);
    
     nvic_irq_enable(TIMER1_IRQn, 5, 0);

    /* enable the TIMER interrupt */
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    /* enable a TIMER */
    
    return 1;
    
}

void TIMER1_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_UP)) 
    {
        //Led_timer_isr();
        //sModKeyMan_Poll_Isr();
        //sModMonitor_Poll_Isr();
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_UP);
    }
}

