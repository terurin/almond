#include "motor.h"
#include "hole.h"
#include "led.h"
#include <driver/pwm.h>
#include "driver/config.h"
#include "driver/uart.h"
#include <driver/pin.h>
#include <driver/int.h>
#include <driver/cn.h>
#include <driver/port.h>
#include <util/qmath.h>


//ホールセンサの状態とPWM出力状態の対応表
//前進
const static pwm_state_name_t table_front[8] = {
    PWM_STATE_CA, //A->AB
    PWM_STATE_CB, //AB->CB
    PWM_STATE_AB, //B->AB
    PWM_STATE_AC, //BC->AC
    PWM_STATE_BC, //C->BC
    PWM_STATE_BA, //CA->BA
    PWM_STATE_FREE//null->free
};

//後退
const static pwm_state_name_t table_back[8] = {
    PWM_STATE_BA, //A->BA
    PWM_STATE_CA, //AB->CA
    PWM_STATE_CB, //B->CB
    PWM_STATE_AB, //BC->AB
    PWM_STATE_AC, //C->AC
    PWM_STATE_BC, //CA->BC
    PWM_STATE_FREE//null->free
};

//ローカルな宣言
static void event_otw(int_id, void*);
static void event_fault(int_id, void*);
static void ctrl_free(void*);
static void ctrl_brake(void*);
static void ctrl_duty(void*);

void motor_init() {
    
    //ドライバの要因割り込みを初期化する
    //set port as readable
    port_read(pin_cast_port(PIN_OTW));
    port_read(pin_cast_port(PIN_FAULT));
    //set as pull up
    change_pull_up(PIN_OTW, true);
    change_pull_up(PIN_FAULT, true);
    //activate interrupt
    pin_set_ppsi(PIN_OTW, PPSI_INT1);
    pin_set_ppsi(PIN_FAULT, PPSI_INT2);
    //interrupt setting
    int_config(INT_ID_1, INT_MODE_FALLING, INT_OTW);
    int_config(INT_ID_2, INT_MODE_FALLING, INT_FAULT);
    //interrupt assign
    int_event(INT_ID_1, event_otw, NULL);
    int_event(INT_ID_2, event_fault, NULL);
    
    //PWMの制御方法を選択する
    pwm_event(ctrl_free,NULL);

}



/*void motor_rate(q15_t duty) {
    hole_t hole = hole_sense();
    q15_t abs = qabs16(duty);
    pwm_rate_write_all(abs);
    pwm_state(duty >= 0 ? table_front[hole] : table_back[hole]);
}

void motor_free() {
    pwm_duty_write_all(0);
    pwm_state(PWM_STATE_FREE);
}*/


static void event_otw(int_id id, void* obj) {
    //motor_free();
    uart_putl("warming over temperature on driver");
    led_off(LED_A);
}

static void event_fault(int_id id, void* obj) {
//    motor_free();
    uart_putl("warming fault on driver");
    led_off(LED_A);
}

static void ctrl_free(void* obj){
    
}

static void ctrl_brake(void* obj){
    
}

static void ctrl_duty(void* obj){
    
}