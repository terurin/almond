#ifndef DRIVER_PORT_HEADER_GUARD
#define DRIVER_PORT_HEADER_GUARD

//全てのPinの名前の定義
enum pin_name {
    // 上位4bit ... port
    // 下位4bit ... pin
    PIN_HA = 0x19,
    PIN_HB = 0x26,
    PIN_HC = 0x27,
    PIN_OTW = 0x28,
    PIN_FALUT = 0x29,
    PIN_PWM_C = 0x10,
    PIN_RST_C = 0x11,
    PIN_PWM_B = 0x12,
    PIN_RST_B = 0x13,
    PIN_LED_A = 0x0A,
    PIN_LED_B = 0x07,
    PIN_VREF = 0x00,
    PIN_RESERVE = 0x01,
    PIN_PGD = 0x10,
    PIN_PGC = 0x11,
    PIN_VM = 0x12,
    PIN_CA = 0x13,
    PIN_CB = 0x20,
    PIN_CC = 0x21,
    PIN_RESERVE2 = 0x22,
    PIN_CLKIN = 0x02,
    PIN_CLKOUT = 0x03,
    PIN_LED_C = 0x08,
    PIN_CS = 0x14,
    PIN_HALT = 0x04,
    PIN_LED_D = 0x09,
    PIN_MISO = 0x23,
    PIN_MOSI = 0x24,
    PIN_MCLK = 0x25,
    PIN_RX = 0x15,
    PIN_TX = 0x16,
    PIN_QEI_Y = 0x17,
    PIN_QEI_X = 0x18
};
typedef enum pin_name pin_name_t;









#endif