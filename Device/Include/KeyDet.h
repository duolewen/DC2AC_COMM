#ifndef KEYDET_H_
#define KEYDET_H_

#include "GlobalDef.h"

// 按键状态定义
typedef enum {
    KEY_STATE_IDLE,           // 空闲状态
    KEY_STATE_IDLE_WAIT,      // 等待按下
    KEY_STATE_PRESSED,        // 按下状态(检测长按)
    KEY_STATE_WAIT_DOUBLE,    // 等待第二次按下(检测双击)
    KEY_STATE_DOUBLE_CONFIRM, // 双击确认
    KEY_STATE_LONGPRESS_WAIT, // 等待按键弹起 返回
} KeyState;

// 按键事件定义
typedef enum {
    KEY_EVENT_NONE,          // 无事件
    KEY_EVENT_PRESS,         // 按下事件(立即触发)
    KEY_EVENT_LONG_PRESS,    // 长按事件(1.5秒)
    KEY_EVENT_DOUBLE_CLICK   // 双击事件
} KeyEvent;

// 按键数据结构

#pragma pack(1)
typedef struct {
    KeyState state;          // 当前状态
    KeyEvent eventLongPress; // 长按
    KeyEvent eventOnePress ; // 单次点击
    uint32_t press_time;      // 按下时间戳
    uint32_t release_time;    // 释放时间戳
    int is_pressed;          // 当前是否按下
    int press_count;         // 按下次数
    uint8_t PressLevel;      //按下的电平
    
    uint32_t debounce_time  ;//消抖时间
    uint8_t last_key_status ;//记录上一次的状态
    
    uint32_t GPIOX;
    uint32_t GPIO_PinX;
} KeyDetector;
#pragma pack()

#pragma pack(1)
typedef struct
{
    uint8_t   step            ;
    uint8_t   LastLevel       ;
    uint32_t  NormalCnt       ;
    uint32_t  NormalHighCnt   ;
    uint32_t  Period_Ms       ;  
    uint32_t  Duty            ;   
    uint32_t  errCnt          ;
    uint8_t   ErrSta          ;
    uint32_t  GPIOx           ;
    uint32_t  GPIO_PINx       ;
}T_MONITOR_CLK_DESC;
#pragma pack()


void key_scan(KeyDetector *key);
void sMonitor_Scan(T_MONITOR_CLK_DESC *pMon );

KeyEvent  key_get_eventLongPress(KeyDetector *key);// 获取按键事件
KeyEvent  key_get_eventOnePress(KeyDetector *key);// 获取按键事件
void      key_clear_eventLongPress(KeyDetector *key);// 获取按键事件
void      key_clear_eventOnePress(KeyDetector *key);// 获取按键事件

#endif
