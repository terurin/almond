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
#include <dsp.h>
//PWM1 Moduleについて記載する

enum pwm_state_name{
    PWM_STATE_AB=0x00,
    PWM_STATE_AC=0x01,
    PWM_STATE_BC=0x02,
    PWM_STATE_BA=0x03,
    PWM_STATE_CA=0x04,
    PWM_STATE_CB=0x05,
    PWM_STATE_END=0x06,//この状態は使わない
    PWM_STATE_LOCK=0x8,
    PWM_STATE_FREE=0x9
};
typedef enum pwm_state_name pwm_state_name_t;

void pwm_init();
uint32_t pwm_cycle();//pwmの制御周波数を取得
uint16_t pwm_period();//pwmの制御周期を取得
//指定した動作波形モードに移行する
void pwm_state(pwm_state_name_t);
//指定したduty比に遷移する 
void pwm_duty_raw(uint16_t);
void pwm_duty(fractional); //(Q0.15 Format)

pwm_state_name_t pwm_state_front(pwm_state_name_t);
pwm_state_name_t pwm_state_back(pwm_state_name_t);
pwm_state_name_t pwm_state_hold(pwm_state_name_t x);
#endif