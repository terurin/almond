#include "led.h"
#include <driver/port.h>

//ledとピン番号の対応表
static const pin_t table[LED_END] ={
    PIN_LED_A,PIN_LED_B,PIN_LED_C,PIN_LED_D
};
static port_t note[LED_END];

void led_init() {
    uint16_t idx;
    for (idx = 0; idx < LED_END; idx++) {
        pin_t number  = table[idx];
        port_t pin = port_cast_pin(number);
        //消灯
        port_direction(pin,false);
        analog_assign(number,false);
        //記録する
        note[idx] =pin;
    }
}

void led_on(led_name_id led) {
    port_set(note[led]);
}

void led_off(led_name_id led) {
     port_clear(note[led]);
}

void led_toggle(led_name_id led){
    port_toggle(note[led]);
}


void led_write(led_name_id led, bool value) {
    port_write(note[led], value);
}

bool led_read(led_name_id led) {
    return port_read(note[led]);
}