/* 作成 terurin
 * 用途　Portの管理
 */
#pragma once
#ifndef DRIVER_PORT_HEADER_GUARD
#define DRIVER_PORT_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
#include "pin.h"
//出力周辺機器IO
enum ppso_name {
    PPSO_NULL = 0b00000,
    PPSO_U1TX = 0b00011,
    PPSO_U1RTS = 0b00100,
    PPSO_SDO1 = 0b00111,
    PPSO_SCK1 = 0b01000,
    PPSO_SS1OUT = 0b01001,
    PPSO_OC1 = 0b10010,
    PPSO_OC2 = 0b10011,
    PPSO_UPDN = 0b11010
};
typedef enum ppso_name ppso_name_t;

//入力周辺機器IO
//参考　データーシート(p46)
enum ppsi_name {
    PPSI_INT1 = 0 << 1 | 1,
    PPSI_INT2 = 1 << 1 | 0,
    //timer
    //input capture
    PPSI_OC_FALUT_A = 11 << 1 | 0,
    PPSI_PWM1_FALUT = 12 << 1 | 0,
    PPSI_PWM2_FALUT = 13 << 1 | 0,
    PPSI_QEIA = 14 << 1 | 0,
    PPSI_QEIB = 14 << 1 | 1,
    PPSI_QEI_INDEX = 15 << 1 | 0,
    PPSI_U1RX = 18 << 1 | 0,
    PPSI_U1CTS = 18 << 1 | 1,
    PPSI_SDI = 20 << 1 | 0,
    PPSI_SCK = 20 << 1 | 1,
    PPSI_SS = 21 << 1 | 0,
    PPSI_END = 21 << 1 | 1, //defect error
};
typedef enum ppsi_name ppsi_name_t;

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