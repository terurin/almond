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

enum pwm_state_name {
    PWM_STATE_AB = 0x00,
    PWM_STATE_AC = 0x01,
    PWM_STATE_BC = 0x02,
    PWM_STATE_BA = 0x03,
    PWM_STATE_CA = 0x04,
    PWM_STATE_CB = 0x05,
    PWM_STATE_END = 0x06, //この状態は使わない
    PWM_STATE_LOCK = 0x8,
    PWM_STATE_FREE = 0x9
};
typedef enum pwm_state_name pwm_state_name_t;

enum pwm_pole_name {
    PWM_POLE_A = 0,
    PWM_POLE_B,
    PWM_POLE_C,
    PWM_POLE_END
};
typedef enum pwm_pole_name pwm_pole_id;

void pwm_init();
uint32_t pwm_cycle(); //pwmの制御周波数を取得
uint16_t pwm_period(); //pwmの制御周期を取得
//指定した動作波形モードに移行する
void pwm_state(pwm_state_name_t);
//指定したduty比に遷移する 
void pwm_duty_write(pwm_pole_id id, uint16_t value);

static inline void pwm_duty_write_all(uint16_t value) {
    pwm_duty_write(PWM_POLE_A, value);
    pwm_duty_write(PWM_POLE_B, value);
    pwm_duty_write(PWM_POLE_C, value);
}
void pwm_rate_write(pwm_pole_id id,fractional rate);//(Q0.15 Format)
void pwm_rate_write_all(fractional rate);
static inline void pwm_rate_each(fractional a, fractional b, fractional c){
    pwm_rate_write(PWM_POLE_A,a);
    pwm_rate_write(PWM_POLE_B,b);
    pwm_rate_write(PWM_POLE_C,c);
            
    
}

pwm_state_name_t pwm_state_front(pwm_state_name_t);
pwm_state_name_t pwm_state_back(pwm_state_name_t);
pwm_state_name_t pwm_state_hold(pwm_state_name_t x);

typedef void (pwm_handler_t(void*));
void pwm_event(pwm_handler_t, void*); //割り込みに関数ポインタを登録
#endif