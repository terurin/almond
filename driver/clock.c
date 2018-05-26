#include "clock.h"
#include <p33Fxxxx.h>

void clock_init() {
    //内部クロックを初期化する

    // Configure PLL prescaler, PLL postscaler, and PLL divisor
    PLLFBD = 41; // M = 43
    CLKDIVbits.PLLPRE = 0; // N1 = 2
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    // Initiate clock switch to internal FRC with PLL (NOSC = 0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    // Wait for clock switch to occur
    while (OSCCONbits.COSC != 0b001);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1) {
    };



}