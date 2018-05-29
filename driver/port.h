/* 作成 terurin
 * 用途　Portの管理
 */
#pragma once
#ifndef DRIVER_PORT_HEADER_GUARD
#define DRIVER_PORT_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
//全てのPinの名前の定義(32bit)
typedef enum {
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
} pin_id;



typedef uint8_t number_t;
typedef uint8_t port_t;
struct pin_pair{
    number_t number;
    port_t port;
};

//基本的なIO
typedef struct pin_pair pin_pair_t;
extern const pin_pair_t pin_pair_error;
bool pin_exist(pin_id);
pin_pair_t pin_cast_pair(pin_id);
static inline bool pin_pair_check(pin_pair_t pair ){
    return (pair.number!=pin_pair_error.number)&&(pair.port!=pin_pair_error.port);//正常ならtrue
}

//周辺機器ID
typedef uint16_t peripheral_id;
extern const peripheral_id peripheral_error;
bool pin_has_peripheral(pin_id);
peripheral_id pin_cast_peripheral(pin_id);
static inline bool peripheral_cheak(peripheral_id p){
    return p!=peripheral_error;
}

//状態変化ピン
typedef uint16_t change_id; //状態変化ピンのID
extern const change_id change_error;
bool pin_has_change(pin_id);
change_id pin_cast_change(pin_id);
static inline bool change_cheak(peripheral_id p){
    return p!=change_error;
}

//アナログID
typedef uint16_t analog_id; //アナログピンのID
extern const analog_id analog_error;
bool pin_has_analog(pin_id);
analog_id pin_cast_analog(pin_id);
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



//basic pin function
//入力ならtrue,出力ならfalse
void pin_direction(pin_pair_t, bool);
void pin_drain(pin_pair_t, bool);
bool pin_read(pin_pair_t);
void pin_write(pin_pair_t pin, bool out);
void pin_set(pin_pair_t);
void pin_clear(pin_pair_t);

//出力先変更
void ppso_assign(pin_id pin, ppso_name_t ppso);
//入力先変更
void ppsi_assign(pin_id pin, ppsi_name_t ppsi);
//アナログ
// flag...trueなら利用する。
void analog_assign(pin_id pin, bool flag);

//状態変化割り込みについて
//true...割り込み有効
void pin_change(pin_id, bool);

//プルアップについて
//true...抵抗を有効化
void pin_pull_up(pin_id, bool);

//デジタル出力として設定する

static inline void pin_dout(pin_id pin) {
    pin_pair_t pair = pin_cast_pair(pin);
    pin_direction(pair, false);
    analog_assign(pin, false);
    pin_write(pair, true);
}

//デジタル入力として設定する。

static inline void pin_din(pin_id pin) {
    pin_pair_t pair = pin_cast_pair(pin);
    pin_direction(pair, true);
    analog_assign(pin, false);
    pin_pull_up(pin, true);
}

//アナログ入力として設定する


//割り込み登録
typedef uint16_t change_t;
typedef void(change_handle_t) (void*);
void change_init();
void change_event(change_handle_t, void*);

#endif