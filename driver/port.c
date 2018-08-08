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
    uint16_t mask = 1u << pin.number;
    volatile uint16_t *target = ports[port];
    uint16_t result = (port<port_max)?(*target&mask):0;
    return result!=0;
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
    uint16_t mask = 1u << pin.number;
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
