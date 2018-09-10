/* 製作　terurin
 * 用途　motorの制御
 */

#ifndef __MID_MOTOR_HEADER_GUARD__
#define __MID_MOTOR_HEADER_GUARD__

#include <util/dsp_type.h>

void motor_init();

//周期的な呼び出しを行うこと
/*void motor_rate(q15_t duty);
void motor_free();
void motor_control();*/
//状態遷移
void motor_free();//開放状態へ遷移
void motor_lock();//静止状態へ遷移
void motor_duty(q15_t rate);



enum drv_status{
    DRV_STATUS_NONE,
    DRV_STATUS_RUN,
    DRV_STATUS_OTW,
    DRV_STATE_FAULT
};
typedef enum drv_status drv_status_t;
drv_status_t drv_info();






#endif