#include "port.h"
#include <p33Fxxxx.h>
#include <stddef.h>

static const size_t port_max = 3;
//PORT周り
static volatile uint16_t * const trises[] = {&TRISA, &TRISB, &TRISC};
static volatile uint16_t * const odcs[] = {&ODCA, &ODCB, &ODCC};
static volatile uint16_t * const ports[] = {&PORTA, &PORTB, &PORTC};
static volatile uint16_t * const lats[] = {&LATA, &LATB, &LATC};

//状態変化割り込み
static change_handle_t* callback_handle = NULL;
static void* callback_object = NULL;

//bit operation

static inline void bits_write(volatile uint16_t* target, uint16_t mask, bool value) {
    if (value) {
        *target |= mask;
    } else {
        *target &= ~mask;
    }
}

void pin_direction(pin_name_t pin, bool flag) {
    uint16_t port = pin_get_port(pin);
    uint16_t mask = 1u << pin_get_number(pin);
    volatile uint16_t *target = trises[port];
    if (port < port_max) {
        bits_write(target, mask, flag);
    }
}

void pin_drain(pin_name_t pin, bool flag) {
    uint16_t port = pin_get_port(pin);
    uint16_t mask = 1u << pin_get_number(pin);
    volatile uint16_t *target = odcs[port];
    if (port < port_max) {
        bits_write(target, mask, flag);
    }
}

bool pin_read(pin_name_t pin) {
    uint16_t port = pin_get_port(pin);
    uint16_t mask = 1u << pin_get_number(pin);
    volatile uint16_t *target = odcs[port];
    if (port < port_max) {
        return *target&mask;
    } else {
        return false;
    }
}

void pin_write(pin_name_t pin, bool flag) {
    uint16_t port = pin_get_port(pin);
    uint16_t mask = 1u << pin_get_number(pin);
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        bits_write(target, mask, flag);
    }
}

void pin_set(pin_name_t pin) {
    uint16_t port = pin_get_port(pin);
    uint16_t mask = 1u << pin_get_number(pin);
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        *target = *target | mask;
    }
}

void pin_clear(pin_name_t pin) {
    uint16_t port = pin_get_port(pin);
    uint16_t mask = 1u << pin_get_number(pin);
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        *target = *target &~mask;
    }
}

void ppso_assign(pin_name_t pin, ppso_name_t ppso) {
    if (pin_has_special(pin)) {
        uint16_t pid = pin_get_special(pin);
        uint8_t* RPRn = (uint8_t*) & RPOR0; //起点を取得
        if (pid < 26) {//RP25まで存在する(データシートより)
            RPRn[pid] = ppso;
        }
    }
}

void ppsi_assign(pin_name_t pin, ppsi_name_t ppsi) {
    if (pin_has_special(pin)) {
        uint8_t* RPRn = (uint8_t*) & RPINR0;
        uint16_t idx = (uint16_t) ppsi;
        if (idx < (uint16_t) PPSI_END) {
            RPRn[idx] = pin_get_special(pin);
        }
    }
}

void analog_assign(pin_name_t pin, bool flag) {
    if (pin_has_analog(pin)) {
        uint16_t num = pin_get_analog(pin);
        uint16_t mask = 1U << num;
        AD1PCFGL = flag ? AD1PCFGL | mask : AD1PCFGL&~mask;
    }
}

void pin_change(pin_name_t pin, bool sw) {
    if (!pin_has_change(pin))return;
    uint16_t num = pin_get_change(pin);
    volatile uint16_t * target = !(num & 0x10) ? &CNEN1 : &CNEN2;
    bits_write(target, num & 0xf, sw);
}

void pin_pull_up(pin_name_t pin, bool sw) {
    if (!pin_has_change(pin))return;
    uint16_t num = pin_get_change(pin);
    volatile uint16_t * target = !(num & 0x10) ? &CNPU1 : &CNPU2;
    bits_write(target, num & 0xf, sw);
}

void change_init() {
    callback_handle = NULL;
    callback_object = NULL;
    IFS1bits.CNIF = false;
    IEC1bits.CNIE = false;
    IPC4bits.CNIP =6; //割り込み優先度
}

void change_event(change_handle_t hwnd, void* obj) {
    callback_handle = hwnd;
    callback_object = obj;
    IFS1bits.CNIF = false;
    IEC1bits.CNIE = hwnd != NULL;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(){
    if (callback_handle!=NULL){
        callback_handle(callback_object);
    }
    IFS1bits.CNIF = false;
}