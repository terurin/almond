/* 作成 terurin
 * 用途　Portの管理
 */
#pragma once
#ifndef DRIVER_PORT_HEADER_GUARD
#define DRIVER_PORT_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
//全てのPinの名前の定義(32bit)
typedef enum pin_number{
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
    PIN_QEI_X
} pin_number_t;

bool pin_number_check(pin_number_t num);

//基本的なIO
typedef uint8_t number_t;
typedef uint8_t port_t;
struct pin{
    number_t number;
    port_t port;
};
typedef struct pin pin_t;
extern const pin_t pin_pair_error;
bool pin_exist(pin_number_t);
pin_t pin_make(pin_number_t);
static inline bool pin_pair_check(pin_t pair ){
    return (pair.number!=pin_pair_error.number)&&(pair.port!=pin_pair_error.port);//正常ならtrue
}

//周辺機器ID
typedef uint16_t peripheral_id;
extern const peripheral_id peripheral_error;
bool pin_has_peripheral(pin_number_t);
peripheral_id pin_cast_peripheral(pin_number_t);
static inline bool peripheral_cheak(peripheral_id p){
    return p!=peripheral_error;
}

//状態変化ピン
typedef uint16_t change_id; //状態変化ピンのID
extern const change_id change_error;
bool pin_has_change(pin_number_t);
change_id pin_cast_change(pin_number_t);
static inline bool change_cheak(peripheral_id p){
    return p!=change_error;
}

//アナログID
typedef uint16_t analog_id; //アナログピンのID
extern const analog_id analog_error;
bool pin_has_analog(pin_number_t);
analog_id pin_cast_analog(pin_number_t);
static inline bool analog_cheak(peripheral_id p){
    return p!=analog_error;
}

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

//basic pin function (高速化のためにpinを用いる)
//入力ならtrue,出力ならfalse
void pin_direction(pin_t, bool);
void pin_drain(pin_t, bool);
bool pin_read(pin_t);
void pin_write(pin_t pin, bool out);
void pin_set(pin_t);
void pin_clear(pin_t);
void pin_toggle(pin_t);

//出力先変更
void ppso_assign(pin_number_t pin, ppso_name_t ppso);
//入力先変更
void ppsi_assign(pin_number_t pin, ppsi_name_t ppsi);
//アナログ
// flag...trueなら利用する。
void analog_assign(pin_number_t pin, bool flag);



//デジタル出力として設定する
static inline void pin_dout(pin_number_t number) {
    pin_t pin = pin_make(number);
    pin_direction(pin, false);
    analog_assign(number, false);
    pin_write(pin, true);
}

//デジタル入力として設定する。
static inline void pin_din(pin_number_t number) {
    pin_t pin = pin_make(number);
    pin_direction(pin, true);
    analog_assign(number, false);

}



//状態変化割り込みについて
//プルアップについて
//割り込み登録

typedef void(change_handle_t) (void*);
void change_init();
void change_pull_up(pin_number_t, bool);//true...抵抗を有効化
void change_assign(pin_number_t, bool);
void change_event(change_handle_t, void*);

#endif