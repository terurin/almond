/* 作成　川上　輝
 * 用途　Portの管理
 */
#ifndef DRIVER_PORT_HEADER_GUARD
#define DRIVER_PORT_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
//全てのPinの名前の定義(32bit)
enum pin_name {
    PIN_UNKNOWN = 0xFFFFFF, //エラー
    //[0..3] ... pin number
    PIN_MASK_NUMBER = 0x00000f,
    //[4..5] ... port
    PIN_SHIFT_PORT = 4,
    PIN_MASK_PORT = 0x3L<<PIN_SHIFT_PORT,
    //[6..10] ...peripheral, not 31
    PIN_SHIFT_SPECIAL = 6,
    PIN_MASK_SPECIAL = 0x1F<<PIN_SHIFT_SPECIAL, 
    PIN_NOT_SPECIAL = 0x1F << PIN_SHIFT_SPECIAL, //特に機能がないピン
    //[11..15] ... cn , not 31
    PIN_SHIFT_CHANGE = 11,
    PIN_MASK_CHANGE = 0x1F<<PIN_SHIFT_CHANGE,
    PIN_NOT_CHANGE = 0x1F<<PIN_SHIFT_CHANGE,
    //[16..15] ... analog , not 15
    PIN_SHIFT_ANALOG = 16,
    PIN_MASK_ANALOG = 0x00F000,
    PIN_NOT_ANALOG = 15L << 12, //アナログピンではないピン
    //1~22
    PIN_HA = 0x19L | 9L << PIN_SHIFT_SPECIAL| 21L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_HB = 0x26L | 22L << PIN_SHIFT_SPECIAL | 18L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_HC = 0x27L | 23L << PIN_SHIFT_SPECIAL | 17L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_OTW = 0x28L | 24L << PIN_SHIFT_SPECIAL | 20L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_FALUT = 0x29L | 25L << PIN_SHIFT_SPECIAL | 19L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    //VSS
    //VCAP
    PIN_PWM_C = 0x10L | 10L <<PIN_SHIFT_SPECIAL  |16L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_RST_C = 0x11L | 11L << PIN_SHIFT_SPECIAL |15L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_PWM_B = 0x12L | 12L << PIN_SHIFT_SPECIAL |14L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_RST_B = 0x13L | 13L << PIN_SHIFT_SPECIAL | 13L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_LED_A = 0x0AL | PIN_NOT_SPECIAL | PIN_NOT_CHANGE| PIN_NOT_ANALOG,
    PIN_LED_B = 0x07L | PIN_NOT_SPECIAL | PIN_NOT_CHANGE| PIN_NOT_ANALOG,
    PIN_PWM_A = 0x1EL | 14L << PIN_SHIFT_SPECIAL |12L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_RST_A = 0x1FL | 15L << PIN_SHIFT_SPECIAL |11L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    //AVSS
    //ADDD
    //MCRL
    PIN_VREF = 0x00L | PIN_NOT_SPECIAL |2L<<PIN_SHIFT_CHANGE| 0L << PIN_SHIFT_ANALOG,
    PIN_RESERVE = 0x01L | PIN_NOT_SPECIAL | 3L<<PIN_SHIFT_CHANGE|1L << PIN_SHIFT_ANALOG,
    PIN_PGD = 0x10L | 0L << PIN_SHIFT_SPECIAL | 4L<<PIN_SHIFT_CHANGE|2L << PIN_SHIFT_ANALOG,
    PIN_PGC = 0x11L | 1L << PIN_SHIFT_SPECIAL |5L<<PIN_SHIFT_CHANGE| 3L << PIN_SHIFT_ANALOG,
    //23~44
    PIN_VM = 0x12L | 2L << PIN_SHIFT_SPECIAL | 6L<<PIN_SHIFT_CHANGE|4L << PIN_SHIFT_ANALOG,
    PIN_CA = 0x13L | 3L << PIN_SHIFT_SPECIAL | 7L<<PIN_SHIFT_CHANGE|5L << PIN_SHIFT_ANALOG,
    PIN_CB = 0x20L | 16L << PIN_SHIFT_SPECIAL |8L<<PIN_SHIFT_CHANGE| 6L << PIN_SHIFT_ANALOG,
    PIN_CC = 0x21L | 17L << PIN_SHIFT_SPECIAL | 9L<<PIN_SHIFT_CHANGE|7L << PIN_SHIFT_ANALOG,
    PIN_RESERVE2 = 0x22L | 18<<PIN_SHIFT_SPECIAL|10L<<PIN_SHIFT_CHANGE|8L << PIN_SHIFT_ANALOG,
    //VDD
    //VSS
    PIN_CLKIN = 0x02L | PIN_NOT_SPECIAL|30L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_CLKOUT = 0x0L | PIN_NOT_SPECIAL|29L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_LED_C = 0x08L | PIN_NOT_SPECIAL| PIN_NOT_CHANGE| PIN_NOT_ANALOG,
    PIN_CS    = 0x14L | 4L << PIN_SHIFT_SPECIAL| 1L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_HALT  = 0x04L | PIN_NOT_SPECIAL| 0L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    PIN_LED_D = 0x09L | PIN_NOT_SPECIAL| PIN_NOT_CHANGE| PIN_NOT_ANALOG,
    PIN_MISO  = 0x23L | 19L << PIN_SHIFT_SPECIAL|28L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_MOSI  = 0x24L | 20L << PIN_SHIFT_SPECIAL|25L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_MCLK  = 0x25L | 21L << PIN_SHIFT_SPECIAL| 26L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG,
    //VSS
    //VDD
    PIN_RX = 0x15L | 5L << PIN_SHIFT_SPECIAL|27L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_TX = 0x16L | 6L << PIN_SHIFT_SPECIAL|24L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_QEI_Y = 0x17L | 7L << PIN_SHIFT_SPECIAL|23L<<PIN_SHIFT_CHANGE| PIN_NOT_ANALOG,
    PIN_QEI_X = 0x18L | 8L << PIN_SHIFT_SPECIAL| 22L<<PIN_SHIFT_CHANGE|PIN_NOT_ANALOG
};
typedef enum pin_name pin_name_t;

enum port_name {
    PORT_A = 0,
    PORT_B,
    PORT_C,
    PORT_UNKNOWN = 0xFF
};
typedef enum port_name port_name_t;

//状態変化ピン
/*enum cn_name{
    
};*/

typedef enum cn_name cn_names;

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

//property
static inline uint16_t pin_get_number(pin_name_t pin) {
    return  pin & PIN_MASK_NUMBER;
}

static inline port_name_t pin_get_port(pin_name_t pin) {
    return ( pin & PIN_MASK_PORT) >> PIN_SHIFT_PORT;
}

//周辺機器ID
static inline uint16_t pin_get_special(pin_name_t pin) {
    return ( pin & PIN_MASK_SPECIAL) >> PIN_SHIFT_SPECIAL;
}

static inline bool pin_has_special(pin_name_t pin){
    return ( pin & PIN_MASK_SPECIAL)!=PIN_NOT_SPECIAL;
}

//状態変化ピン
static inline uint16_t pin_get_change(pin_name_t pin){
    return (pin&PIN_MASK_CHANGE)>>PIN_SHIFT_CHANGE;
}

static inline bool pin_has_change(pin_name_t pin){
    return (pin&PIN_MASK_CHANGE)==PIN_NOT_CHANGE;
}

//アナログID
static inline uint16_t pin_get_analog(pin_name_t pin) {
    return ( pin & PIN_MASK_ANALOG) >> PIN_SHIFT_ANALOG;
}

static inline bool pin_has_analog(pin_name_t pin){
    return ( pin & PIN_MASK_ANALOG) !=PIN_NOT_ANALOG;
}

//basic pin function
//入力ならtrue,出力ならfalse
void pin_dir(pin_name_t, bool);
void pin_drain(pin_name_t,bool);
bool pin_read(pin_name_t);
void pin_write(pin_name_t pin, bool out);
void pin_set(pin_name_t);
void pin_clr(pin_name_t);

//出力先変更
void ppso_assing(pin_name_t pin, ppso_name_t ppso);
//入力先変更
void ppsi_assing(pin_name_t pin, ppsi_name_t ppsi);
//アナログ
// flag...trueなら利用する。
void analog_assing(pin_name_t pin, bool flag);

//状態変化割り込みについて
//true...割り込み有効
void pin_change(pin_name_t,bool);

//プルアップについて
//true...抵抗を有効化
void pin_pull_up(pin_name_t,bool);



#endif