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
#include <util/dsp_type.h>

//PWM1 Moduleについて記載する

enum pwm_state_name {
    PWM_STATE_AB = 0x00,
    PWM_STATE_AC = 0x01,
    PWM_STATE_BC = 0x02,
    PWM_STATE_BA = 0x03,
    PWM_STATE_CA = 0x04,
    PWM_STATE_CB = 0x05,
    PWM_STATE_LOCK = 0x6,
    PWM_STATE_FREE = 0x7,
    PWM_STATE_END
};
typedef enum pwm_state_name pwm_state_name_t;

/*enum pwm_pole_name {
    PWM_POLE_A = 0,
    PWM_POLE_B,
    PWM_POLE_C,
    PWM_POLE_END//終端処理
};
typedef enum pwm_pole_name pwm_pole_id;*/

void pwm_init();
uint32_t pwm_cycle(); //pwmの制御周波数を取得
uint16_t pwm_period(); //pwmの制御周期を取得
q16_t pwm_duty();//実行中のduty比

void pwm_write(pwm_state_name_t state, q16_t rate);
//よく使うであろう処理の別名
#define pwm_write_free() pwm_write(PWM_STATE_FREE,0)
#define pmw_write_lock() pwm_write(PWM_STATE_LOCK,0)

typedef void (pwm_handler_t(void*));
void pwm_event(pwm_handler_t, void*); //割り込みに関数ポインタを登録
#endif