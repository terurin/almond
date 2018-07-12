#include "clock.h"
#include <p33Fxxxx.h>
#include "config.h"
#include <stdbool.h>

#define FCY_DEFALUT (7.37*MHZ)

//設計メモ
//動作周波数は80MHzであり、FCY=40[MHz]である

//初期設定
//内部クロック(7.37MHz)


//外部クロック(8MHz)の設定
//PLLPRE =  2
//PLLDIV = 40
//PLLPORT =  2

static void enter_hs_pll();
static void enter_frc_pll();

void clock_init() {
    //内部クロックを初期化する

#if !EXTERNAL_CLOCK
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
#else
    //shift FCY 80MHz
    enter_hs_pll();
#endif
}

static void enter_frc_pll(){
    //clock setting(PLL)
    CLKDIVbits.PLLPRE = 2; //2
    PLLFBDbits.PLLDIV = 40;
    CLKDIVbits.PLLPOST = 2; //2
    OSCCONbits.CLKLOCK = false; //Lock
    //内蔵クロックの微調整
    OSCTUNbits.TUN= +9; 
    //クロックを変更する
    __builtin_write_OSCCONH(0x01); //FRC with PLL
    __builtin_write_OSCCONL(0x01); //Source Change
    //適用確認
    while (OSCCONbits.COSC != 0b001);
    while (OSCCONbits.LOCK != 1); //wait
}

static void enter_hs_pll() {
    //clock setting(PLL)
    CLKDIVbits.PLLPRE = 2; //2
    PLLFBDbits.PLLDIV = 40;
    CLKDIVbits.PLLPOST = 2; //2
    OSCCONbits.CLKLOCK = false; //Lock

    //unlock clock source
    __builtin_write_OSCCONH(0x03); //HS with PLL
    __builtin_write_OSCCONL(0x01); //Source Change
    //適用確認
    while (OSCCONbits.COSC != 0b011);
    while (OSCCONbits.LOCK != 1);
}