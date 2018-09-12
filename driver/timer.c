#include "timer.h"

#include <timer.h>
#include <stdbool.h>
#include "config.h"

//制約　内部クロックは80MHzで動作していると仮定する
const uint32_t timer1_tps = FCY;
const tick16_t timer1_max = UINT16_MAX;
const uint32_t timer23_tps=FCY;
const tick32_t timer23_max = FCY;
static uint16_t timer23_sec; //[sec]

//マクロ
#define T1_FLAG(x) IFS0bits.T1IF=(x)
#define T2_FLAG(x) IFS0bits.T2IF=(x)

void timer_init() {
    timer1_init();
    timer23_init();
}

void timer1_init() {
    const T1CONBITS con = {
        .TON = false,
        .TSIDL = false, //running at idel
        .TGATE = false,
        .TCKPS = 0, //priscaler 1:1
        .TSYNC = false,
        .TCS = false//internal clock
    };

    //設定
    T1CONbits = con;
    TMR1 = 0;
    T1CONbits.TON = true;
}

void timer23_init() {

    //Timer 2,3ともに利用する

    const T2CONBITS con = {
        .TON = false, // 意図的に切っておく
        .TSIDL = false, //running at idel
        .TGATE = false,
        .TCKPS = 1, //priscale 1:1
        .TCS = false, //internal clock
    };

    //管理境域初期化
    timer23_sec = 0;
    //設定
    T2CONbits = con;
    TMR3 = 0, TMR2 = 0;
    PR3 = timer23_max >> 16, PR2 = timer23_max;

    //割り込み開始
    T2_FLAG(false);
    IEC0bits.T2IE = true;
    IPC1bits.T2IP = TMR23_PRI;
    //タイマー起動
    T2CONbits.TON = true;
}

tick16_t timer1_tick() {
    return TMR1;
}

tick32_t timer23_tick() {
    return (uint32_t) TMR3 << 16 | TMR2;
}

sec32_t timer23_seconds() {
    return timer23_sec;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt() {
    timer23_sec++;
    T2_FLAG(false);
}

void stopwatch16_init_timer1(stopwatch16_t* obj){
    stopwatch16_init(obj,timer1_tick,timer1_max);
}