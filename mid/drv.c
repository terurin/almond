#include "drv.h"
#include "driver/port.h"
#include "motor.h"
#include "driver/config.h"
#include "driver/uart.h"
#include <driver/pin.h>
#include <driver/int.h>
#include "led.h"
//ローカルな宣言
static void drv_init_report();
static void event_otw(int_id,void*);
static void event_fault(int_id,void*);

void drv_init(){
    //依存先
    motor_init();
    drv_init_report();
    //起動したことを明示
    led_on(LED_A);
}


static void drv_init_report(){
    //set port as readable
    port_read(pin_cast_port(PIN_OTW));
    port_read(pin_cast_port(PIN_FAULT));
    //set as pull up
    change_pull_up(PIN_OTW,true);
    change_pull_up(PIN_FAULT,true);
    //activate interrupt
    pin_set_ppsi(PIN_OTW,PPSI_INT1);
    pin_set_ppsi(PIN_FAULT,PPSI_INT2);
    //interrupt setting
    int_config(INT_ID_1,INT_MODE_FALLING,INT_OTW);
    int_config(INT_ID_2,INT_MODE_FALLING,INT_FAULT);
    //interrupt assign
    int_event(INT_ID_1,event_otw,NULL);
    int_event(INT_ID_2,event_fault,NULL);
}

static void event_otw(int_id id,void* obj){
    motor_free();
    uart_putl("warming over temperature on driver");
    led_off(LED_A);
}
static void event_fault(int_id id,void* obj){
    motor_free();
    uart_putl("warming fault on driver");
    led_off(LED_A);
}