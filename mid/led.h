/* 作成　terurin
 * 用途 ledの操作
 */

#pragma once
#ifndef MID_LED_HEADER_GUARD
#define MID_LED_HEADER_GUARD
#include <stdbool.h>

enum led_name{
    LED_A=0,
    LED_B,
    LED_C,
    LED_D    
};
typedef enum led_name led_name_t;


void led_init();
void led_on(led_name_t);
void led_off(led_name_t);
void led_write(led_name_t led,bool value);
bool led_read(led_name_t);

static inline void led_toggle(led_name_t led){
    led_write(led,led_read(led));
}
#endif