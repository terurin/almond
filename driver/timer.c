#include "timer.h"

#include <timer.h>
#include <stdbool.h>
#include "config.h"

//制約　内部クロックは80MHzで動作していると仮定する
#define FOSC (80000000UL)
#define FCY (FOSC/2)
uint32_t timer_fcy=FCY;
const tick_t tick_max= FCY*60;//60s=1min
uint16_t timer23_minutes;//[min]

//マクロ
#define T2_FLAG(x) IFS0bits.T2IF=(x)

void timer23_init(){
    
    //Timer 2,3ともに利用する
    
    const T2CONBITS con={
      .TON=false,// 意図的に切っておく
      .TSIDL=false, //running at idel
      .TGATE=false,
      .TCKPS=1,//priscale 1:1
      .TCS=false,//internal clock
    };
    
    //管理境域初期化
    timer23_minutes=0;
    //設定
    T2CONbits=con;
    TMR3=0,TMR2=0;
    PR3=tick_max>>16,PR2=tick_max;
            
    //割り込み開始
    T2_FLAG(false);
    IEC0bits.T2IE=true;
    IPC1bits.T2IP=TMR23_PRI;
    //タイマー起動
    T2CONbits.TON=true;
}

tick_t timer23_clock(){
    return (uint32_t)TMR3<<16|TMR2;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(){
    //
    timer23_minutes++;
    T2_FLAG(false);
}