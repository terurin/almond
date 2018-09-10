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
    motor_free();
}

//開放状態の動作

static void ctrl_free(void* obj) {
    pwm_write_free();
}

void motor_free() {
    pwm_event(ctrl_free, NULL);
}

//静止時の動作

static void ctrl_lock(void* obj) {
    pwm_write_lock();
}

void motor_lock() {
    pwm_event_default(ctrl_lock);
}

//固定回転数で駆動
static void ctrl_duty(void* obj){
    const hole_t hole = hole_sense(); 
    const q15_t x= (obj!=NULL)?*(int16_t*)obj:0;
    const pwm_state_name_t next=(x>=0)?table_front[hole]:table_back[hole];
    const q16_t abs = abs15_s(x);
    pwm_write(next,abs);
   
}

void motor_duty(q15_t rate){
    static q15_t instance;
    instance=rate;//stackから静的領域にコピーする
    pwm_event(ctrl_duty,&rate);
}



//エラーに対応させる
static void event_otw(int_id id, void* obj) {
    pwm_write_free();
    motor_free();
    uart_putl("warming over temperature on driver");
    led_off(LED_A);
}

static void event_fault(int_id id, void* obj) {
    pwm_write_free();
    motor_free();
    uart_putl("warming fault on driver");
    led_off(LED_A);
}