/* 作成 terurin
 * 用途　Portの管理
 */
#pragma once
#ifndef DRIVER_PORT_HEADER_GUARD
#define DRIVER_PORT_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
#include "pin.h"

void port_init();

//basic pin function (高速化のためにpinを用いる)
//入力ならtrue,出力ならfalse
void port_direction(port_t, bool);
void port_drain(port_t, bool);
bool port_read(port_t);
void port_write(port_t pin, bool out);
void port_set(port_t);
void port_clear(port_t);
void port_toggle(port_t);




//デジタル出力として設定する
static inline void port_dout(pin_t number) {
    port_t pin = port_cast_pin(number);
    port_direction(pin, false);
    pin_set_analog(number, false);
    port_write(pin, true);
}

//デジタル入力として設定する。
static inline void port_din(pin_t number) {
    port_t pin = port_cast_pin(number);
    port_direction(pin, true);
    pin_set_analog(number, false);

}



#endif