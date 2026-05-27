#include "KeyDet.h"

// 时间常量 (毫秒)
#define DEBOUNCE_TIME 50      // 消抖时间
#define LONG_PRESS_TIME 1500  // 长按时间1.5秒
#define DOUBLE_CLICK_TIME 500 // 双击间隔400ms(更符合实际)


// 按键扫描函数 (需要周期调用，比如每10ms调用一次)
void key_scan(KeyDetector *key)
{
    uint8_t current_key_status =    gpio_input_bit_get(key->GPIOX,key->GPIO_PinX);
    // 消抖处理
    if (current_key_status != key->last_key_status)
    {
        key->debounce_time += 10;
    }
    else
    {
        key->debounce_time = 0;
    }
    
    if (key->debounce_time > DEBOUNCE_TIME) 
    {
        key->is_pressed = (current_key_status == key->PressLevel);
        key->last_key_status = current_key_status;
    }
    
    switch (key->state) // 状态机处理
    {   
        case KEY_STATE_IDLE:
        {
            if (!key->is_pressed) // 等待释放
            {
                key->release_time = 0;
                key->state = KEY_STATE_IDLE_WAIT;
            } 
        }
            break;
        case KEY_STATE_IDLE_WAIT:
        {
            if (key->is_pressed)
            {
                key->press_time = 0;
                key->state = KEY_STATE_PRESSED;
                key->press_count = 1;
                key->eventOnePress = KEY_EVENT_PRESS; // 立即触发按下事件
            }
        }
             break;
        case KEY_STATE_PRESSED:
        {
            key->press_time += 10;
            if (!key->is_pressed) // 在长按时间前释放，进入等待双击状态
            {
                key->release_time = 0;
                key->state = KEY_STATE_WAIT_DOUBLE;
            } 
            else if (key->press_time > LONG_PRESS_TIME) // 长按达到1.5秒，触发长按事件
            {
                key->state = KEY_STATE_LONGPRESS_WAIT;
                key->eventLongPress = KEY_EVENT_LONG_PRESS;
            }
        }
            break;
        case KEY_STATE_WAIT_DOUBLE:
        {
            key->release_time += 10;
            if (key->is_pressed)   // 第二次按下，确认双击
            {
                key->press_time  = 0;
                key->state = KEY_STATE_DOUBLE_CONFIRM;
                key->press_count = 2;
            } 
            else if (key->release_time > DOUBLE_CLICK_TIME) // 超时未第二次按下，回到空闲状态  这里不触发任何事件，因为按下事件已经在第一次按下时触发了
            {
                key->state = KEY_STATE_IDLE;
            }
        }
            break;
        case KEY_STATE_DOUBLE_CONFIRM:
        {
            key->press_time += 10;
            if (!key->is_pressed) 
            {
                key->state = KEY_STATE_IDLE;
                //key->event = KEY_EVENT_DOUBLE_CLICK;
            }
            else if (key->press_time > DOUBLE_CLICK_TIME) // 超时未第二次按下，回到空闲状态  这里不触发任何事件，因为按下事件已经在第一次按下时触发了
            {
                key->state = KEY_STATE_IDLE;
            }
        }
            break;
        case KEY_STATE_LONGPRESS_WAIT:
        {
            if (!key->is_pressed) // 等待释放
            {
                key->release_time = 0;
                key->state = KEY_STATE_IDLE;
            } 
        }
            break;
    }
}

void sMonitor_Scan(T_MONITOR_CLK_DESC *pMon )
{
       uint8_t ThisLevel = gpio_input_bit_get(pMon->GPIOx,pMon->GPIO_PINx);
       if(pMon->step == 0)
       {
         if(pMon->LastLevel == 0 && ThisLevel == 1)
         {
            pMon->NormalHighCnt = 0;
            pMon->NormalCnt = 0;
            pMon->step = 1;
         }
       }
       else if(pMon->step == 1)
       {
            pMon->NormalCnt += 1;
            if(ThisLevel == 1)
                 pMon->NormalHighCnt += 1;
            if(pMon->LastLevel == 0 && ThisLevel == 1)
            {
                pMon->Period_Ms = pMon->NormalCnt ;
                pMon->Duty      = (float)pMon->NormalHighCnt / (float)pMon->Period_Ms *100;      
                
                if((pMon->Period_Ms >= 900 || pMon->Period_Ms <= 1100)
                    ||(pMon->Duty >= 45 || pMon->Duty <= 55))
                {
                    pMon->errCnt = 0; //清除计数
                    pMon->ErrSta = 0;
                }
                
                pMon->step = 0;
                
            }
       }
       pMon->LastLevel = ThisLevel ;
   
       pMon->errCnt += 1;
       if(pMon->errCnt > 3000 )//3 秒都没有 翻转 报 故障
       {
            pMon->errCnt = 0;

            pMon->Period_Ms = 0 ;
            pMon->Duty      = 0;      
            pMon->ErrSta = 1;
            pMon->step = 0;
       }
       
}

// 获取按键事件
KeyEvent key_get_eventLongPress(KeyDetector *key)
{
    KeyEvent event = key->eventLongPress;
    return event;
}

// 获取按键事件
KeyEvent key_get_eventOnePress(KeyDetector *key) 
{
    KeyEvent event = key->eventOnePress;
    return event;
}

// 获取按键事件
void  key_clear_eventLongPress(KeyDetector *key) 
{
    key->eventLongPress = KEY_EVENT_NONE; //清除事件
}

// 获取按键事件
void  key_clear_eventOnePress(KeyDetector *key) 
{
    key->eventOnePress = KEY_EVENT_NONE; //清除事件
}




