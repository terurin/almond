/* 製作　terurin
 * 用途　motorの制御
 */

#ifndef __MID_MOTOR_HEADER_GUARD__
#define __MID_MOTOR_HEADER_GUARD__

#include <util/dsp_type.h>

void motor_init();

//周期的な呼び出しを行うこと
void motor_pwm(q15_t duty);




void motor_control();







#endif