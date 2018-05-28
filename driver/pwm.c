#include "pwm.h"
#include <p33Fxxxx.h>
#include "clock.h"

/* 用語メモ
 * HB ... Half Bridge の略 (H,L,Free の三つの状態がある) 
 */

/* 技術ノート
 * dspic.PWMxH=DRV.PWMx
 * dspic.PWMxL=DRV.RESET
 */

//各種設定
#define KHZ (1000)
static uint32_t cycle = 10 * KHZ;
//すべてのHBを開放する
static const P1OVDCONBITS ov_free = {
    //PWM1
    .POVD1H = false, .POVD1L = false, //use pwm module outputs?
    .POUT1H = false, .POUT1L = true, //manual output
    //PWM2
    .POVD2H = false, .POVD2L = false, //use pwm module outputs?
    .POUT2H = false, .POUT2L = true, //manual output
    //PWM3
    .POVD3H = false, .POVD3L = false, //use pwm module outputs?
    .POUT3H = false, .POUT3L = true, //manual output
};

//すべてのHBを用いてブレーキをかける。
static const P1OVDCONBITS ov_lock ={
    //PWM1
    .POVD1H = false, .POVD1L = false, //use pwm module outputs?
    .POUT1H = false, .POUT1L = false, //manual output
    //PWM2
    .POVD2H = false, .POVD2L = false, //use pwm module outputs?
    .POUT2H = false, .POUT2L = false, //manual output
    //PWM3
    .POVD3H = false, .POVD3L = false, //use pwm module outputs?
    .POUT3H = false, .POUT3L = false, //manual output
};

//矩形制御用
static const P1OVDCONBITS ov_states[] = {
    //STATE A->B
    {
        //PWM1
        .POVD1H = true, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = false, //manual output
        //PWM2
        .POVD2H = false, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = false, //manual output
        //PWM3
        .POVD3H = false, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = true, //manual output

    },
    //STATE A->C
    {
        //PWM1
        .POVD1H = true, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = false, //manual output
        //PWM2
        .POVD2H = false, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = true, //manual output
        //PWM3
        .POVD3H = false, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = false, //manual output

    },
    //STATE B->C
    {
        //PWM1
        .POVD1H = false, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = true, //manual output
        //PWM2
        .POVD2H = true, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = false, //manual output
        //PWM3
        .POVD3H = false, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = false, //manual output

    },
    //STATE B->A
    {
        //PWM1
        .POVD1H = false, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = false, //manual output
        //PWM2
        .POVD2H = true, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = false, //manual output
        //PWM3
        .POVD3H = false, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = true, //manual output

    },
    //STATE C->A
    {
        //PWM1
        .POVD1H = false, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = false, //manual output
        //PWM2
        .POVD2H = false, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = true, //manual output
        //PWM3
        .POVD3H = true, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = false, //manual output
    },
    //STATE C->B
    {
        //PWM1
        .POVD1H = false, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = true, //manual output
        //PWM2
        .POVD2H = false, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = false, //manual output
        //PWM3
        .POVD3H = true, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = false, //manual output
    }
};

//周期レジスタの値を計算する
static inline uint16_t free_period() {
    return clock_fcy() / (cycle) - 1;
}

static inline uint16_t up_down_period() {
    return clock_fcy() / (cycle * 2) - 1;
}

void pwm_init() {
    //定数定義

    const PTCONBITS pt = {
        .PTEN = false, //安全のために切っておく
        .PTSIDL = false, // at idel, working
        .PTOPS = 0b0000, //post scaler
        .PTCKPS = 0b00, //priscaler
        .PTMOD = 0b10, //up-down mode
    };

    const PWM1CON1BITS pwm1 = {
        //PWM1 独立　Hのみ
        .PMOD1 = true,
        .PEN1H = true,
        .PEN1L = false,
        //PWM2 独立　Hのみ
        .PMOD2 = true,
        .PEN2H = true,
        .PEN2L = false,
        //PWM3 独立　Hのみ        
        .PMOD3 = true,
        .PEN3H = true,
        .PEN3L = false,
    };

    const PWM2CON2BITS pwm2 = {
        .SEVOPS = 0x9, //interrupt post scaler ,division by (1<<n)
        .IUE = false, //sync DCx on PWM-Time base
        .OSYNC = false, //sync PxOVDCON on PWM-Time base
    };

    const P1DTCON1BITS p1dt = {
        .DTAPS = 0b00, //post scaler divide by 1^n on dead time
        .DTA = 0,
        .DTBPS = 0b00, //post scaler divide by 1^n on dead time
        .DTB = 0
    };

    const P1DTCON2BITS p2dt = {
        .DTS1A = false, //use dead time on module A
        .DTS1I = false, //use dead time on module A
    };

    //各種レジスタを初期化する
    PTCONbits = pt;
    PWM1CON1bits = pwm1;
    PWM2CON2bits = pwm2;
    P1DTCON1bits = p1dt;
    P1DTCON2bits = p2dt;
    P1OVDCONbits = ov_free;
    //カウンタを初期化する
    PTPER = up_down_period();
    PTMR = 0;
    P1DC1 = P1DC2 = P1DC3 = 0;
    //割り込みを許可する
    IFS3bits.PWM1IF = false;
    IPC14bits.PWM1IP = 6; //割り込み優先度(up to 7)
    IEC3bits.PWM1IE = true;
    //起動
    PTCONbits.PTEN = true;
}

uint32_t pwm_cycle() {
    return cycle;
}

void __attribute__((interrupt, no_auto_psv)) _MPWM1Interrupt() {
    //TODO 後で書く
    IFS3bits.PWM1IF = false;
}