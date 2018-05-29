#include "led.h"
#include <driver/port.h>

//ledのidとpinのidの対応表
static const pin_id led_list[] = {
    PIN_LED_A,
    PIN_LED_B,
    PIN_LED_C,
    PIN_LED_D
};

static const uint16_t led_size = sizeof (led_list) / sizeof (led_list[0]);

static inline pin_id cast(led_name_t led) {
    return led_list[(uint16_t) led];
}

void led_init() {
    uint16_t idx;
    for (idx = 0; idx < led_size; idx++) {
        pin_id pin = cast(idx);
        pin_pair_t pair = pin_cast_pair(pin);
        pin_direction(pair, false);
        analog_assign(pin, false);
        pin_clear(pair);
    }
}

void led_on(led_name_t led) {
    pin_id pin = cast(led);
    pin_pair_t pair = pin_cast_pair(pin);
    pin_set(pair);
}

void led_off(led_name_t led) {
    pin_id pin = cast(led);
    pin_pair_t pair = pin_cast_pair(pin);
    pin_clear(pair);
}

void led_write(led_name_t led, bool value) {
    pin_id pin = cast(led);
    pin_pair_t pair = pin_cast_pair(pin);
    return pin_write(pair, value);
}

bool led_read(led_name_t led) {
    pin_id pin = cast(led);
    pin_pair_t pair = pin_cast_pair(pin);
    return pin_read(pair);
}