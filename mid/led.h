/* 作成　terurin
 * 用途 ledの操作
 */

#pragma once
#ifndef MID_LED_HEADER_GUARD
#define MID_LED_HEADER_GUARD
#include <stdbool.h>


enum led_name {
    LED_A = 0,
    LED_B,
    LED_C,
    LED_D,
    LED_END//終端処理用
};
typedef enum led_name led_name_id;


void led_init();
void led_on(led_name_id);
void led_off(led_name_id);
void led_toggle(led_name_id);
void led_write(led_name_id led, bool value);
bool led_read(led_name_id);
#endif