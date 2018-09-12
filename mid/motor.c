#include "motor.h"
#include "hole.h"
#include "led.h"
#include <driver/pwm.h>
#include "driver/config.h"
#include "driver/uart.h"
#include "driver/timer.h"
#include <driver/pin.h>
#include <driver/int.h>
#include <driver/cn.h>
#include <driver/port.h>

#include"control.h"


//ローカルな宣言
static void event_otw(int_id, void*);
static void event_fault(int_id, void*);

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

//開放状態の動作

void motor_free() {
    free_enter();
}

//静止時の動作

void motor_lock() {
    lock_enter();
}

//固定回転数で駆動

void motor_duty(q15_t rate) {
    duty_enter(rate);
}

//強制的に回す制御

void motor_force(tick32_t period, q15_t duty) {
    force_enter(period, duty);
}
