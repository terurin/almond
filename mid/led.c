#include "led.h"
#include <driver/port.h>

//ledのidとpinのidの対応表
static const pin_name_t led_list[]={
   PIN_LED_A,
   PIN_LED_B,
   PIN_LED_C,
   PIN_LED_D
};

static const uint16_t led_size=sizeof(led_list)/sizeof(led_list[0]);

static inline pin_name_t cast(led_name_t led){
    return led_list[(uint16_t)led];
}

void led_init(){
    uint16_t idx;
    for (idx=0;idx<led_size;idx++){
        pin_name_t pin = cast(led_list[idx]);
        pin_direction(pin,false);
        analog_assign(pin,false);
        pin_clear(pin);
    }
}

void led_on(led_name_t led){
    pin_name_t pin = cast(led);
    pin_set(pin);
}

void led_off(led_name_t led){
    pin_name_t pin = cast(led);
    pin_clear(pin);
}

void led_write(led_name_t led,bool value){
    pin_name_t pin = cast(led);
    return pin_write(pin,value);
}

bool led_read(led_name_t led){
    pin_name_t pin = cast(led);
    return pin_read(pin);
}