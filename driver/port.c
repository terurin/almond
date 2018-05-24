#include "port.h"
#include <p33Fxxxx.h>
#include <stddef.h>

void pin_dir(pin_name_t pin, bool dir) {
    port_name_t port = pin_get_port(pin);
    uint16_t id = pin_get_id(pin);
    uint16_t mask = 1u << id;
    switch (port) {
        case PORT_A:
            TRISA = dir ? TRISA | mask : TRISA&~mask;
            return;
        case PORT_B:
            TRISB = dir ? TRISB | mask : TRISB&~mask;
            return;
        case PORT_C:
            TRISC = dir ? TRISC | mask : TRISC&~mask;
            return;
        default:
            return;
    }
}

bool pin_read(pin_name_t pin) {
    port_name_t port = pin_get_port(pin);
    uint16_t id = pin_get_id(pin);
    uint16_t mask = 1u << id;
    
    switch (port){
        case PORT_A:
            return PORT_A&mask;
        case PORT_B:
            return PORT_B&mask;
        case PORT_C:
            return PORT_C&mask;
        default:
            return false;
    }
}

void pin_set (pin_name_t pin){
    port_name_t port = pin_get_port(pin);
    uint16_t id = pin_get_id(pin);
    uint16_t mask = 1u << id;
    
    switch (port){
        case PORT_A:
            LATA|=mask;
            return;
        case PORT_B:
            LATB|=mask;
            return ;
        case PORT_C:
            LATB|=mask;
            return;
        default:
            return ;
    }
}

void pin_clr(pin_name_t pin){
    port_name_t port = pin_get_port(pin);
    uint16_t id = pin_get_id(pin);
    uint16_t mask = 1u << id;
    
    switch (port){
        case PORT_A:
            LATA&=~mask;
            return;
        case PORT_B:
            LATB&=~mask;
            return ;
        case PORT_C:
            LATB&=~mask;
            return;
        default:
            return ;
    }
}