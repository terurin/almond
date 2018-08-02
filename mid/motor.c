#include "motor.h"
#include "hole.h"
#include <driver/pwm.h>
#include <util/qmath.h>
//ホールセンサの状態とPWM出力状態の対応表

//前進
const static pwm_state_name_t table_front[8]={
    PWM_STATE_CA,//A->AB
    PWM_STATE_CB,//AB->CB
    PWM_STATE_AB,//B->AB
    PWM_STATE_AC,//BC->AC
    PWM_STATE_BC,//C->BC
    PWM_STATE_BA,//CA->BA
    PWM_STATE_FREE//null->free
};

//後退
const static pwm_state_name_t table_back[8]={
    PWM_STATE_BA,//A->BA
    PWM_STATE_CA,//AB->CA
    PWM_STATE_CB,//B->CB
    PWM_STATE_AB,//BC->AB
    PWM_STATE_AC,//C->AC
    PWM_STATE_BC,//CA->BC
    PWM_STATE_FREE//null->free
};

void motor_init(){
    
}

void motor_rate(q15_t duty){
    hole_t hole = hole_sense();
    q15_t abs = qabs16(duty);
    pwm_rate_write_all(abs);
    pwm_state(duty>=0?table_front[hole]:table_back[hole]);
}

void motor_free(){
    pwm_duty_write_all(0);
    pwm_state(PWM_STATE_FREE);
}