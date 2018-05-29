#include "port.h"
#include <p33Fxxxx.h>
#include <stddef.h>
#include <stdlib.h>

static const size_t port_max = 3;
//PORT周り
static volatile uint16_t * const trises[] = {&TRISA, &TRISB, &TRISC};
static volatile uint16_t * const odcs[] = {&ODCA, &ODCB, &ODCC};
static volatile uint16_t * const ports[] = {&PORTA, &PORTB, &PORTC};
static volatile uint16_t * const lats[] = {&LATA, &LATB, &LATC};

//状態変化割り込み
static change_handle_t* callback_handle = NULL;
static void* callback_object = NULL;

//ピンのテーブル

struct pin_info {
    //1Word
    unsigned number : 4; //
    unsigned port : 2;
    unsigned : 1;
    unsigned exist : 1;
    unsigned analog : 4;
    unsigned has_analog : 1;
    unsigned : 3;
    //2Word
    unsigned peripheral : 5;
    unsigned has_peripheral : 1;
    unsigned : 2;
    unsigned change : 5;
    unsigned has_change : 1;
    unsigned : 3;
};
typedef struct pin_info pin_info_t;

const static size_t pin_table_count = 45; //要素数
const static pin_info_t pin_table[] = {
    {.exist = false},
    //HA
    {.exist = true, .port = 1, .number = 9, .has_analog = false,
        .has_peripheral = true, .peripheral = 9, .has_change = true, .change = 21},
    //HB
    {.exist = true, .port = 2, .number = 6, .has_analog = false,
        .has_peripheral = true, .peripheral = 22, .has_change = true, .change = 18},
    //HC
    {.exist = true, .port = 2, .number = 7, .has_analog = false,
        .has_peripheral = true, .peripheral = 23, .has_change = true, .change = 17},
    //OTW
    {.exist = true, .port = 2, .number = 8, .has_analog = false,
        .has_peripheral = false, .has_change = true, .change = 20},
    //FALUT
    {.exist = true, .port = 2, .number = 9, .has_analog = false,
        .has_peripheral = false, .has_change = true, .change = 19},
    //VSS
    {.exist = false},
    //VCAP
    {.exist = false},
    //PWM_C
    {.exist = true, .port = 1, .number = 10, .has_analog = false,
        .has_peripheral = true, .peripheral = 10, .has_change = true, .change = 16},
    //RST_C
    {.exist = true, .port = 1, .number = 11, .has_analog = false,
        .has_peripheral = true, .peripheral = 11, .has_change = true, .change = 15},
    //PWM_B
    {.exist = true, .port = 1, .number = 12, .has_analog = false,
        .has_peripheral = true, .peripheral = 12, .has_change = true, .change = 14},
    //RST_B
    {.exist = true, .port = 1, .number = 13, .has_analog = false,
        .has_peripheral = true, .peripheral = 13, .has_change = true, .change = 13},
    //LED_A
    {.exist = true, .port = 0, .number = 10, .has_analog = false,
        .has_peripheral = false, .has_change = false},
    //LED_B
    {.exist = true, .port = 0, .number = 7, .has_analog = false,
        .has_peripheral = false, .has_change = false},
    //PWM_A
    {.exist = true, .port = 1, .number = 14, .has_analog = false,
        .has_peripheral = true, .peripheral = 14, .has_change = true, .change = 12},
    //RST_A
    {.exist = true, .port = 1, .number = 15, .has_analog = false,
        .has_peripheral = true, .peripheral = 15, .has_change = true, .change = 11},
    //AVSS
    {.exist = false},
    //AVDD
    {.exist = false},
    //MCRL
    {.exist = false},
    //VREF
    {.exist = true, .port = 0, .number = 0, .has_analog = true, .analog = 0,
        .has_peripheral = false, .has_change = true, .change = 2},
    //RESERVE
    {.exist = true, .port = 0, .number = 1, .has_analog = true, .analog = 1,
        .has_peripheral = false, .has_change = true, .change = 3},
    //PGD
    {.exist = true, .port = 1, .number = 0, .has_analog = true, .analog = 2,
        .has_peripheral = true, .peripheral = 0, .has_change = true, .change = 4},
    //PGC
    {.exist = true, .port = 1, .number = 1, .has_analog = true, .analog = 3,
        .has_peripheral = true, .peripheral = 1, .has_change = true, .change = 5},
    //VM
    {.exist = true, .port = 1, .number = 2, .has_analog = true, .analog = 4,
        .has_peripheral = true, .peripheral = 2, .has_change = true, .change = 6},
    //CA
    {.exist = true, .port = 1, .number = 3, .has_analog = true, .analog = 5,
        .has_peripheral = true, .peripheral = 3, .has_change = true, .change = 7},
    //CB
    {.exist = true, .port = 2, .number = 0, .has_analog = true, .analog = 6,
        .has_peripheral = true, .peripheral = 16, .has_change = true, .change = 8},
    //CC
    {.exist = true, .port = 2, .number = 1, .has_analog = true, .analog = 7,
        .has_peripheral = true, .peripheral = 17, .has_change = true, .change = 9},
    //RESERVE#2
    {.exist = true, .port = 2, .number = 2, .has_analog = true, .analog = 8,
        .has_peripheral = true, .peripheral = 18, .has_change = true, .change = 10},
    //VDD
    {.exist = false},
    //VSS
    {.exist = false},
    //CLKIN
    {.exist = true, .port = 0, .number = 2, .has_analog = false,
        .has_peripheral = false, .has_change = true, .change = 29},
    //CLKOUT
    {.exist = true, .port = 0, .number = 3, .has_analog = false,
        .has_peripheral = false, .has_change = true, .change = 30},
    //LED_C
    {.exist = true, .port = 0, .number = 8, .has_analog = false,
        .has_peripheral = false, .has_change = false},
    //CS
    {.exist = true, .port = 1, .number = 4, .has_analog = false,
        .has_peripheral = true, .peripheral = 4, .has_change = true, .change = 1},
    //HALT
    {.exist = true, .port = 0, .number = 4, .has_analog = false,
        .has_peripheral = false, .has_change = true, .change = 0},
    //LED_D
    {.exist = true, .port = 0, .number = 9, .has_analog = false,
        .has_peripheral = false, .has_change = false},
    //MISO
    {.exist = true, .port = 2, .number = 3, .has_analog = false,
        .has_peripheral = true, .peripheral = 19, .has_change = true, .change = 28},
    //MOSI
    {.exist = true, .port = 2, .number = 4, .has_analog = false,
        .has_peripheral = true, .peripheral = 20, .has_change = true, .change = 25},
    //MCLK
    {.exist = true, .port = 2, .number = 5, .has_analog = false,
        .has_peripheral = true, .peripheral = 21, .has_change = true, .change = 26},
    //VSS
    {.exist = false},
    //VDD
    {.exist = false},
    //RX
    {.exist = true, .port = 1, .number = 5, .has_analog = false,
        .has_peripheral = true, .peripheral = 5, .has_change = true, .change = 27},
    //TX
    {.exist = true, .port = 1, .number = 6, .has_analog = false,
        .has_peripheral = true, .peripheral = 6, .has_change = true, .change = 24},
    //QEI_Y
    {.exist = true, .port = 1, .number = 7, .has_analog = false,
        .has_peripheral = true, .peripheral = 7, .has_change = true, .change = 23},
    //QEI_X
    {.exist = true, .port = 1, .number = 8, .has_analog = false,
        .has_peripheral = true, .peripheral = 8, .has_change = true, .change = 22},
};
//エラー検知用の定数
const pin_pair_t pin_pair_error = {
    .number = 0xff, .port = 0xff
};

const peripheral_id peripheral_error= 0xffff;
const change_id change_error=0xffff;
const analog_id analog_error=0xffff;

static inline const pin_info_t* get_info(pin_id id) {
    if (id < pin_table_count) {
        return pin_table+id;
    } else {
        return NULL;
    }
}

bool pin_exist(pin_id id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->exist;
    } else {
        return false;
    }
}

pin_pair_t pin_cast_pair(pin_id id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        if (info->exist) {
            pin_pair_t pair = {.number = info->number, .port = info->port};
            return pair;
        } else {
            return pin_pair_error;
        }
    } else {
        return pin_pair_error;
    }
}

bool pin_has_peripheral(pin_id id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->has_peripheral;
    } else {
        return false;
    }
}

peripheral_id pin_cast_peripheral(pin_id id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        if (info->has_peripheral) {
            return info->peripheral;
        } else {
            return peripheral_error;
        }
    } else {
        return peripheral_error;
    }
}

bool pin_has_change(pin_id id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->has_change;
    } else {
        return false;
    }
}

change_t pin_cast_change(pin_id id){
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        if (info->has_change) {
            return info->change;
        } else {
            return change_error;
        }
    } else {
        return change_error;
    }
}

bool pin_has_analog(pin_id id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->has_analog;
    } else {
        return false;
    }
}

analog_id pin_cast_analog(pin_id id){
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        if (info->has_change) {
            return info->analog;
        } else {
            return change_error;
        }
    } else {
        return change_error;
    }
}

//bit operation

static inline void bits_write(volatile uint16_t* target, uint16_t mask, bool value) {
    if (value) {
        *target |= mask;
    } else {
        *target &= ~mask;
    }
}

void pin_direction(pin_pair_t pin, bool flag) {
    uint16_t port = pin.port;
    uint16_t mask = 1U<<pin.number;
    volatile uint16_t *target = trises[port];
    if (port < port_max) {
        bits_write(target, mask, flag);
    }
}

void pin_drain(pin_pair_t pin, bool flag) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = odcs[port];
    if (port < port_max) {
        bits_write(target, mask, flag);
    }
}

bool pin_read(pin_pair_t pin) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.port;
    volatile uint16_t *target = odcs[port];
    if (port < port_max) {
        return *target&mask;
    } else {
        return false;
    }
}

void pin_write(pin_pair_t pin, bool flag) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        bits_write(target, mask, flag);
    }
}

void pin_set(pin_pair_t pin) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.port;
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        *target = *target | mask;
    }
}

void pin_clear(pin_pair_t pin) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        *target = *target &~mask;
    }
}

void ppso_assign(pin_id pin, ppso_name_t ppso) {
    if (pin_has_peripheral(pin)) {
        peripheral_id pid = pin_cast_peripheral(pin);
        uint8_t* RPRn = (uint8_t*) & RPOR0; //起点を取得
        if (pid < 26) {//RP25まで存在する(データシートより)
            RPRn[pid] = ppso;
        }
    }
}

void ppsi_assign(pin_id pin, ppsi_name_t ppsi) {
    if (pin_has_peripheral(pin)) {
        uint8_t* RPRn = (uint8_t*) & RPINR0;
        uint16_t idx = (uint16_t) ppsi;
        if (idx < (uint16_t) PPSI_END) {
            RPRn[idx] = pin_cast_peripheral(pin);
        }
    }
}

void analog_assign(pin_id pin, bool flag) {
    if (pin_has_analog(pin)) {
        uint16_t num = pin_cast_analog(pin);
        uint16_t mask = 1U << num;
        AD1PCFGL = flag ? AD1PCFGL | mask : AD1PCFGL&~mask;
    }
}

void pin_change(pin_id pin, bool sw) {
    if (!pin_has_change(pin))return;
    uint16_t num = pin_cast_change(pin);
    volatile uint16_t * target = !(num & 0x10) ? &CNEN1 : &CNEN2;
    bits_write(target, num & 0xf, sw);
}

void pin_pull_up(pin_id pin, bool sw) {
    if (!pin_has_change(pin))return;
    change_id num = pin_cast_change(pin);
    volatile uint16_t * target = !(num & 0x10) ? &CNPU1 : &CNPU2;
    bits_write(target, num & 0xf, sw);
}

void change_init() {
    callback_handle = NULL;
    callback_object = NULL;
    IFS1bits.CNIF = false;
    IEC1bits.CNIE = false;
    IPC4bits.CNIP = 6; //割り込み優先度
}

void change_event(change_handle_t hwnd, void* obj) {
    callback_handle = hwnd;
    callback_object = obj;
    IFS1bits.CNIF = false;
    IEC1bits.CNIE = hwnd != NULL;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt() {
    if (callback_handle != NULL) {
        callback_handle(callback_object);
    }
    IFS1bits.CNIF = false;
}