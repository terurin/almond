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
port_t pin_cast_port(pin_t);
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

//出力先変更
void pin_set_ppso(pin_t pin, ppso_name_t ppso);
//入力先変更
void pin_set_ppsi(pin_t pin, ppsi_name_t ppsi);
//アナログ
// flag...trueなら利用する。
void pin_set_analog(pin_t pin, bool flag);


#endif