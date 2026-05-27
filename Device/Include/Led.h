#ifndef LED_H_
#define LED_H_

#include "GlobalDef.h"

// LED状态枚举
typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} led_state_t;

// LED工作模式枚举
typedef enum {
    MODE_OFF = 0,           // 常灭
    MODE_ON = 1,            // 常亮
    MODE_BLINK_CONT = 2,    // 持续闪烁
    MODE_BLINK_TIMES = 3    // 指定次数闪烁
} led_mode_t;


// 1ms定时器回调函数（由系统定时器调用）
void        Led_timer_isr(void);
void        Led_set_mode(led_mode_t mode);
void        Led_blink_times(uint8_t times);
void        Led_toggle_times(uint32_t ontimes,uint32_t OffTime);
void        Led_set_blink_timing(uint16_t on_ms, uint16_t off_ms);
led_state_t Led_get_state(void);
bool        Led_is_blink_complete(void);
#endif
