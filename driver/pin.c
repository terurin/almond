#include "pin.h"

//エラー検知用の定数
const port_t pin_pair_error = {
    .number = 0xff, .port = 0xff
};
const peripheral_id peripheral_error= 0xffff;
const change_id change_error=0xffff;
const analog_id analog_error=0xffff;

//ピンのテーブル
//const static size_t pin_table_count = 45; //要素数
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

static inline const pin_info_t* get_info(pin_t id) {
    if (id < PIN_END) {
        return pin_table+id;
    } else {
        return NULL;
    }
}

bool pin_number_check(pin_t num){
    if(0<=num&&num<PIN_END){
        return pin_table[num].exist;
    }else{
        return false;
    }
}

bool pin_has_port(pin_t id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->exist;
    } else {
        return false;
    }
}

port_t port_cast_pin(pin_t id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        if (info->exist) {
            port_t pair = {.number = info->number, .port = info->port};
            return pair;
        } else {
            return pin_pair_error;
        }
    } else {
        return pin_pair_error;
    }
}

bool pin_has_peripheral(pin_t id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->has_peripheral;
    } else {
        return false;
    }
}

peripheral_id pin_cast_peripheral(pin_t id) {
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

bool pin_has_change(pin_t id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->has_change;
    } else {
        return false;
    }
}

change_id pin_cast_change(pin_t id){
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

bool pin_has_analog(pin_t id) {
    const pin_info_t* info = get_info(id);
    if (info != NULL) {
        return info->has_analog;
    } else {
        return false;
    }
}

analog_id pin_cast_analog(pin_t id){
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