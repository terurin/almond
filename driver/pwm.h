/* 作成者　terurin
 * 用途　pwmの出力
 * DRV8322用に記述している
 *  */

#pragma once
#ifndef DRIVER_PWM_HEADER_GUARD
#define DRIVER_PWM_HEADER_GUARD

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//PWM1 Moduleについて記載する

enum pwm_state{
    PWM_STATE_AB=0x00,
    PWM_STATE_AC=0x01,
    PWM_STATE_BC=0x02,
    PWM_STATE_BA=0x03,
    PWM_STATE_CA=0x04,
    PWM_STATE_CB=0x05,
    PWM_STATE_LOCK=0x8,
    PWM_STATE_FREE=0x9
};
typedef enum pwm_state pwm_state_t;

void pwm_init();
uint32_t pwm_cycle();//pwmの制御周波数を取得







#endif