#include "port.h"
#include <p33Fxxxx.h>
#include <stddef.h>
#include <stdlib.h>
#include <util/bits.h>
static const size_t port_max = 3;
//PORT周り
static volatile uint16_t * const trises[] = {&TRISA, &TRISB, &TRISC};
static volatile uint16_t * const odcs[] = {&ODCA, &ODCB, &ODCC};
static volatile uint16_t * const ports[] = {&PORTA, &PORTB, &PORTC};
static volatile uint16_t * const lats[] = {&LATA, &LATB, &LATC};

void port_init(){
   
    


    
}

void port_direction(port_t pin, bool flag) {
    uint16_t port = pin.port;
    uint16_t mask = 1U<<pin.number;
    volatile uint16_t *target = trises[port];
    if (port < port_max) {
        bits_write_reg(target, mask, flag);
    }
}

void port_drain(port_t pin, bool flag) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = odcs[port];
    if (port < port_max) {
        bits_write_reg(target, mask, flag);
    }
}

bool port_read(port_t pin) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.port;
    volatile uint16_t *target = odcs[port];
    if (port < port_max) {
        return *target&mask;
    } else {
        return false;
    }
}

void port_write(port_t pin, bool flag) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        bits_write_reg(target, mask, flag);
    }
}

void port_set(port_t pin) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.port;
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        *target = *target | mask;
    }
}

void port_clear(port_t pin) {
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        *target = *target &~mask;
    }
}

void port_toggle(port_t pin){
    uint16_t port = pin.port;
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = lats[port];
    if (port < port_max) {
        *target = *target ^mask;
    }
}

void port_ppso(pin_t pin, ppso_name_t ppso) {
    if (pin_has_peripheral(pin)) {
        peripheral_id pid = pin_cast_peripheral(pin);
        uint8_t* RPRn = (uint8_t*) & RPOR0; //起点を取得
        if (pid < 26) {//RP25まで存在する(データシートより)
            RPRn[pid] = ppso;
        }
    }
}

void port_ppsi(pin_t pin, ppsi_name_t ppsi) {
    if (pin_has_peripheral(pin)) {
        uint8_t* RPRn = (uint8_t*) & RPINR0;
        uint16_t idx = (uint16_t) ppsi;
        if (idx < (uint16_t) PPSI_END) {
            RPRn[idx] = pin_cast_peripheral(pin);
        }
    }
}

void analog_assign(pin_t pin, bool flag) {
    if (pin_has_analog(pin)) {
        uint16_t num = pin_cast_analog(pin);
        uint16_t mask = 1U << num;
        AD1PCFGL = flag ? AD1PCFGL | mask : AD1PCFGL&~mask;
    }
}



