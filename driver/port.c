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

void ppso_assing(pin_name_t pin,ppso_name_t ppso){
    uint16_t pid = pin_get_pid(pin);
    uint8_t* RPRn = (uint8_t*)&RPOR0;//起点を取得
    if (pid<26){//RP25まで存在する(データシートより)
        RPRn[pid]=ppso;
    }
}

void ppsi_assing(pin_name_t pin,ppsi_name_t ppsi){
    uint8_t* RPRn=(uint8_t*)&RPINR0;
    uint16_t idx =(uint16_t) ppsi;
    if (idx<(uint16_t)PPSI_END){
        RPRn[idx]= pin_get_pid(pin);
    }
}