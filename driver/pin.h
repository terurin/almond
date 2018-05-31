/* 作成者　terurin
 * 用途　各ピン情報を抽象化する
 */

#pragma once
#ifndef DRIVER_PIN_HEADER_GUARD
#define DRIVER_PIN_HEADER_GUARD

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
//全てのPinの名前の定義(32bit)
typedef enum pin{
    PIN_NONE = 0, //for error
    PIN_HA = 1,
    PIN_HB,
    PIN_HC,
    PIN_OTW,
    PIN_FALUT,
    //VSS
    //VCAP
    PIN_PWM_C = 8,
    PIN_RST_C,
    PIN_PWM_B,
    PIN_RST_B,
    PIN_LED_A,
    PIN_LED_B,
    PIN_PWM_A,
    PIN_RST_A,
    //AVSS
    //ADDD
    //MCRL
    PIN_VREF = 19,
    PIN_RESERVE,
    PIN_PGD,
    PIN_PGC,
    //23~44
    PIN_VM = 23,
    PIN_CA,
    PIN_CB,
    PIN_CC,
    PIN_RESERVE2,
    //VDD
    //VSS
    PIN_CLKIN = 30,
    PIN_CLKOUT,
    PIN_LED_C,
    PIN_CS,
    PIN_HALT,
    PIN_LED_D,
    PIN_MISO,
    PIN_MOSI,
    PIN_MCLK,
    //VSS
    //VDD
    PIN_RX = 41,
    PIN_TX,
    PIN_QEI_Y,
    PIN_QEI_X,
    PIN_END //終端処理
} pin_t;

bool pin_number_check(pin_t num);

//基本的なIO
typedef uint8_t port_number_t;
typedef uint8_t port_id;
struct port{
    port_number_t number;
    port_id port;
};
typedef struct port port_t;
extern const port_t pin_pair_error;
bool pin_has_port(pin_t);
port_t port_cast_pin(pin_t);
static inline bool port_check(port_t pair ){
    return (pair.number!=pin_pair_error.number)&&(pair.port!=pin_pair_error.port);//正常ならtrue
}

//周辺機器ID
typedef uint16_t peripheral_id;
extern const peripheral_id peripheral_error;
bool pin_has_peripheral(pin_t);
peripheral_id pin_cast_peripheral(pin_t);
static inline bool peripheral_cheak(peripheral_id p){
    return p!=peripheral_error;
}

//状態変化ピン
typedef uint16_t change_id; //状態変化ピンのID
extern const change_id change_error;
bool pin_has_change(pin_t);
change_id pin_cast_change(pin_t);
static inline bool change_cheak(peripheral_id p){
    return p!=change_error;
}

//アナログID
typedef uint16_t analog_id; //アナログピンのID
extern const analog_id analog_error;
bool pin_has_analog(pin_t);
analog_id pin_cast_analog(pin_t);
static inline bool analog_cheak(peripheral_id p){
    return p!=analog_error;
}

#endif