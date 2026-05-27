#include "Led.h"

// LED控制结构体
typedef struct {
    led_state_t state;      // 当前LED状态
    led_mode_t mode;        // 当前工作模式
    uint32_t counter;       // 计时计数器
    uint16_t on_time;       // 点亮时间(ms)
    uint16_t off_time;      // 熄灭时间(ms)
    uint8_t blink_count;    // 闪烁次数计数
    uint8_t target_count;   // 目标闪烁次数
    bool blink_complete;    // 闪烁完成标志
} led_controller_t;

// 全局LED控制器实例
static led_controller_t led_ctrl = {
    .state = LED_OFF,
    .mode = MODE_OFF,
    .counter = 0,
    .on_time = 500,    // 2秒亮
    .off_time = 500,   // 1秒灭
    .blink_count = 0,
    .target_count = 0,
    .blink_complete = false
};

// 硬件LED控制函数（需要根据实际平台实现）
static void hardware_led_set(led_state_t state)
{
    if (state == LED_ON) {
        // 实际硬件操作 - 点亮LED
            LED1_ON;
    } else {
        // 实际硬件操作 - 熄灭LED
            LED1_OFF;
    }
}

// 1ms定时器回调函数（由系统定时器调用）
void Led_timer_isr(void)
{
    led_controller_t *led = &led_ctrl;
    
    switch (led->mode) {
        case MODE_OFF:
            if (led->state != LED_OFF) {
                led->state = LED_OFF;
                hardware_led_set(LED_OFF);
            }
            break;
            
        case MODE_ON:
            if (led->state != LED_ON) {
                led->state = LED_ON;
                hardware_led_set(LED_ON);
            }
            break;
            
        case MODE_BLINK_CONT:
            // 持续闪烁模式
            if (led->state == LED_ON) {
                if (++led->counter >= led->on_time) {
                    led->state = LED_OFF;
                    led->counter = 0;
                    hardware_led_set(LED_OFF);
                }
            } else {
                if (++led->counter >= led->off_time) {
                    led->state = LED_ON;
                    led->counter = 0;
                    hardware_led_set(LED_ON);
                }
            }
            break;
            
        case MODE_BLINK_TIMES:
            // 指定次数闪烁模式
            if (led->blink_complete) {
                // 完成闪烁后的停顿
                if (++led->counter >= 1000) { // 1秒停顿
                    led->blink_complete = false;
                    led->blink_count = 0;
                    led->counter = 0;
                }
            } else {
                if (led->state == LED_ON) {
                    if (++led->counter >= 200) { // 200ms亮
                        led->state = LED_OFF;
                        led->counter = 0;
                        hardware_led_set(LED_OFF);
                        led->blink_count++;
                        
                        if (led->blink_count >= led->target_count) {
                            led->blink_complete = true;
                            led->counter = 0;
                        }
                    }
                } else {
                    if (++led->counter >= 200) { // 200ms灭
                        led->state = LED_ON;
                        led->counter = 0;
                        hardware_led_set(LED_ON);
                    }
                }
            }
            break;
    }
}

// API函数：设置LED模式
void Led_set_mode(led_mode_t mode)
{
    led_ctrl.mode = mode;
    led_ctrl.counter = 0;
    led_ctrl.blink_count = 0;
    led_ctrl.blink_complete = false;
    
    // 立即更新LED状态
    if (mode == MODE_OFF) {
        led_ctrl.state = LED_OFF;
        hardware_led_set(LED_OFF);
    } else if (mode == MODE_ON) {
        led_ctrl.state = LED_ON;
        hardware_led_set(LED_ON);
    }
}

// API函数：设置闪烁次数
void Led_blink_times(uint8_t times)
{
    led_ctrl.target_count = times;
    led_ctrl.mode = MODE_BLINK_TIMES;
    led_ctrl.counter = 0;
    led_ctrl.blink_count = 0;
    led_ctrl.blink_complete = false;
    led_ctrl.state = LED_OFF; // 从熄灭开始
    hardware_led_set(LED_OFF);
}

// API函数：设置闪烁次数
void Led_toggle_times(uint32_t ontimes,uint32_t OffTime)
{
    led_ctrl.on_time  = ontimes;       // 点亮时间(ms)
    led_ctrl.off_time = OffTime;      // 熄灭时间(ms)
       
}

// API函数：设置闪烁时间参数
void Led_set_blink_timing(uint16_t on_ms, uint16_t off_ms)
{
    led_ctrl.on_time = on_ms;
    led_ctrl.off_time = off_ms;
}

// API函数：获取LED状态
led_state_t Led_get_state(void)
{
    return led_ctrl.state;
}

// API函数：检查闪烁是否完成（仅对MODE_BLINK_TIMES有效）
bool Led_is_blink_complete(void)
{
    return led_ctrl.blink_complete;
}
