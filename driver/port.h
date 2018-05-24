/* 作成　川上　輝
 * 用途　Portの管理
 */
#ifndef DRIVER_PORT_HEADER_GUARD
#define DRIVER_PORT_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
//全てのPinの名前の定義
enum pin_name {
    PIN_UNKNOWN = 0xFFFF, //エラー
    //[0..3] ... pin number
    PIN_MASK_NUMBER = 0x000f,
    //[4..5] ... port
    PIN_MASK_PORT = 0x0030,
    PIN_SHIFT_PORT = 4,
    //[6..10] ...peripheral, not 31
    PIN_MASK_SPECIAL = 0x07C0,
    PIN_SHIFT_SPECIAL = 6,
    PIN_NORMAL = 31 << PIN_SHIFT_SPECIAL, //特に機能がないピン
    //[12..15] ... analog , not 15
    PIN_MASK_ANALOG = 0xF000,
    PIN_SHIFT_ANALOG = 12,
    PIN_DIGITAL = 15 << 12, //アナログピンではないピン
    //1~22
    PIN_HA = 0x19 | 9 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_HB = 0x26 | 22 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_HC = 0x27 | 23 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_OTW = 0x28 | 24 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_FALUT = 0x29 | 25 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    //VSS
    //VCAP
    PIN_PWM_C = 0x10 | 10 <<PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_RST_C = 0x11 | 11 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_PWM_B = 0x12 | 12 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_RST_B = 0x13 | 13 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_LED_A = 0x0A | PIN_NORMAL | PIN_DIGITAL,
    PIN_LED_B = 0x07 | PIN_NORMAL | PIN_DIGITAL,
    PIN_PWM_A = 0x1E | 14 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    PIN_RST_A = 0x1F | 15 << PIN_SHIFT_SPECIAL | PIN_DIGITAL,
    //AVSS
    //ADDD
    //MCRL
    PIN_VREF = 0x00 | PIN_NORMAL | 0 << PIN_SHIFT_ANALOG,
    PIN_RESERVE = 0x01 | PIN_NORMAL | 1 << PIN_SHIFT_ANALOG,
    PIN_PGD = 0x10 | 0 << PIN_SHIFT_SPECIAL | 2 << PIN_SHIFT_ANALOG,
    PIN_PGC = 0x11 | 1 << PIN_SHIFT_SPECIAL | 3 << PIN_SHIFT_ANALOG,
    //23~44
    PIN_VM = 0x12 | 2 << PIN_SHIFT_SPECIAL | 4 << PIN_SHIFT_ANALOG,
    PIN_CA = 0x13 | 3 << PIN_SHIFT_SPECIAL | 5 << PIN_SHIFT_ANALOG,
    PIN_CB = 0x20 | 16 << PIN_SHIFT_SPECIAL | 6 << PIN_SHIFT_ANALOG,
    PIN_CC = 0x21 | 17 << PIN_SHIFT_SPECIAL | 7 << PIN_SHIFT_ANALOG,
    PIN_RESERVE2 = 0x22 | 18<<PIN_SHIFT_SPECIAL|8 << PIN_SHIFT_ANALOG,
    PIN_CLKIN = 0x02 | PIN_NORMAL| PIN_DIGITAL,
    PIN_CLKOUT = 0x0 | PIN_NORMAL| PIN_DIGITAL,
    PIN_LED_C = 0x08 | PIN_NORMAL| PIN_DIGITAL,
    PIN_CS = 0x14 | 4 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_HALT = 0x04 | PIN_NORMAL| PIN_DIGITAL,
    PIN_LED_D = 0x09 | PIN_NORMAL| PIN_DIGITAL,
    PIN_MISO = 0x23 | 19 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_MOSI = 0x24 | 20 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_MCLK = 0x25 | 21 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_RX = 0x15 | 5 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_TX = 0x16 | 6 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_QEI_Y = 0x17 | 7 << PIN_SHIFT_SPECIAL| PIN_DIGITAL,
    PIN_QEI_X = 0x18 | 8 << PIN_SHIFT_SPECIAL| PIN_DIGITAL
};
typedef enum pin_name pin_name_t;

enum port_name {
    PORT_A = 0,
    PORT_B,
    PORT_C,
    PORT_UNKNOWN = 0xFF
};
typedef enum port_name port_name_t;

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
    PPSI_END = 21 << 1 | 1,//defect error
};
typedef enum ppsi_name ppsi_name_t;

//methods

static inline port_name_t pin_get_port(pin_name_t pin) {
    return ((uint16_t) pin & (uint16_t)PIN_MASK_PORT) >> PIN_SHIFT_PORT;
}

static inline uint16_t pin_get_number(pin_name_t pin) {
    return (uint16_t) pin & (uint16_t)PIN_MASK_NUMBER;
}
//周辺機器ID
static inline uint16_t pin_get_special(pin_name_t pin) {
    return ((uint16_t) pin & (uint16_t)PIN_MASK_SPECIAL) >> PIN_SHIFT_SPECIAL;
}

static inline uint16_t pin_has_special(pin_name_t pin){
    return ((uint16_t) pin & (uint16_t)PIN_MASK_SPECIAL)!=(uint16_t)PIN_NORMAL;
}

//アナログID
static inline uint16_t pin_get_analog(pin_name_t pin) {
    return ((uint16_t) pin & (uint16_t)PIN_MASK_ANALOG) >> PIN_SHIFT_ANALOG;
}

static inline uint16_t pin_has_analog(pin_name_t pin){
    return ((uint16_t) pin & (uint16_t)PIN_MASK_ANALOG) !=PIN_DIGITAL;
}

//basic pin function
//入力ならtrue,出力ならfalse
void pin_dir(pin_name_t, bool);
bool pin_read(pin_name_t);
void pin_set(pin_name_t);
void pin_clr(pin_name_t);

static inline void pin_write(pin_name_t pin, bool out) {
    out ? pin_set(pin) : pin_clr(pin);
}

//出力先変更
void ppso_assing(pin_name_t pin, ppso_name_t ppso);
//入力先変更
void ppsi_assing(pin_name_t pin, ppsi_name_t ppsi);
//アナログ

// flag...trueなら利用する。
void analog_assing(pin_name_t pin, bool flag);



#endif