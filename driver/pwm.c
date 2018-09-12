#include "pwm.h"
#include <p33Fxxxx.h>
#include "config.h"
#include <util/qmath.h>

/* 用語メモ
 * HB ... Half Bridge の略 (H,L,Free の三つの状態がある) 
 * アップダウン モード...低速回転を行えるようになるが、制御精度が1/2になる
 * フリーラン　モード　...通常のPWM出力
 */

/* 技術ノート
 * dspic.PWMxH=DRV.PWMx
 * dspic.PWMxL=DRV.RESET
 */

//各種設定
#define UP_DOWN (true) //PWMの動作モード　アップダウンならtrue,フリーランならfalse
#define CYCLE (10.0e3)
static const uint32_t cycle = CYCLE;
#if UP_DOWN == true
static const uint16_t period = FCY / CYCLE / 2;
#else
static const uint16_t period = FCY / CYCLE;
#endif

static q16_t duty_max = QCAST(0.9f, 16); //dutyの上限
static q16_t duty_min = QCAST(0.1f, 16); //duty比の下限(安全対策)
static q16_t duty = 0; //現在のduty比
static pwm_state_name_t state; //現在のstate

//内部テーブル
//static volatile uint16_t * const dc_table[] = {&P1DC1, &P1DC2, &P1DC3};

//割り込み　コールバック用
static pwm_handler_t* callback_handler = NULL;
static void *callback_object = NULL;

//矩形制御用
static const P1OVDCONBITS ov_table[] = {
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
        .POUT1H = false, .POUT1L = true, //manual output
        //PWM2
        .POVD2H = false, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = false, //manual output
        //PWM3
        .POVD3H = false, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = true, //manual output

    },
    //STATE B->C
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
    //STATE B->A
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
    //STATE C->A
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
    },
    //STATE C->B
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
    //STATE LOCK
    {
        //PWM1
        .POVD1H = false, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = true, //manual output
        //PWM2
        .POVD2H = false, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = true, //manual output
        //PWM3
        .POVD3H = false, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = true, //manual output
    },
    //STATE FREE
    {
        //PWM1
        .POVD1H = false, .POVD1L = false, //use pwm module outputs?
        .POUT1H = false, .POUT1L = false, //manual output
        //PWM2
        .POVD2H = false, .POVD2L = false, //use pwm module outputs?
        .POUT2H = false, .POUT2L = false, //manual output
        //PWM3
        .POVD3H = false, .POVD3L = false, //use pwm module outputs?
        .POUT3H = false, .POUT3L = false, //manual output
    }
};

void pwm_init() {
    //定数定義

    const PTCONBITS pt = {
        .PTEN = false, //安全のために切っておく
        .PTSIDL = false, // at idel, working
        .PTOPS = 0b0000, //post scaler
        .PTCKPS = 0b00, //priscaler
#if UP_DOWN == true
        .PTMOD = 0b10, //up-down mode
#else
        .PTMOD = 0b00, //free-run mode
#endif
    };

    const PWM1CON1BITS pwm1 = {
        //PWM1 独立　Hのみ
        .PMOD1 = true,
        .PEN1H = true,
        .PEN1L = true,
        //PWM2 独立　Hのみ
        .PMOD2 = true,
        .PEN2H = true,
        .PEN2L = true,
        //PWM3 独立　Hのみ        
        .PMOD3 = true,
        .PEN3H = true,
        .PEN3L = true,
    };

    const PWM2CON2BITS pwm2 = {
        .SEVOPS = 0x3, //interrupt post scaler ,division by (2^n)
        .IUE = true, //sync DCx on PWM-Time base
        .OSYNC = true, //sync PxOVDCON on PWM-Time base
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
    //カウンタを初期化する
    PTPER = period;
    PTMR = 0;
    pwm_write_free();
    //割り込みを許可する
    IFS3bits.PWM1IF = false;
    IPC14bits.PWM1IP = PWM_PRI; //割り込み優先度(up to 7)
    IEC3bits.PWM1IE = false;
    //起動
    PTCONbits.PTEN = true;
}

uint32_t pwm_cycle() {
    return cycle;
}

uint16_t pwm_period() {
    return period;
}

q16_t pwm_duty() {
    return duty;
}

pwm_state_name_t pwm_state() {
    return state;
}

void pwm_write(pwm_state_name_t mode, q16_t rate) {
    //波形パターンを選択する
    const P1OVDCONBITS patten = mode < PWM_STATE_END ? ov_table[mode] : ov_table[PWM_STATE_FREE];
    //制限域に入れる
    const q16_t limited = clip16(rate, duty_max, duty_min);
    const uint16_t raw = ((uint32_t) limited * period) >> 16;
    //書き込み
    P1OVDCONbits = patten;
    P1DC1 = P1DC2 = P1DC3 = raw;
    //更新
    duty = limited;
    state=mode;
}

void pwm_event(pwm_handler_t hwnd, void* obj) {
    callback_handler = hwnd;
    callback_object = obj;
    IFS3bits.PWM1IF = false;
    IEC3bits.PWM1IE = hwnd != NULL;
}

void __attribute__((interrupt, no_auto_psv)) _MPWM1Interrupt() {
    if (callback_handler != NULL) {
        callback_handler(callback_object);
    }
    IFS3bits.PWM1IF = false;
}

pwm_state_name_t pwm_state_next(pwm_state_name_t state) {
    const static pwm_state_name_t list [] = {
        PWM_STATE_AC,
        PWM_STATE_BC,
        PWM_STATE_BA,
        PWM_STATE_CA,
        PWM_STATE_CB,
        PWM_STATE_AB
    };
    const int idx = (int) state;
    return idx < PWM_STATE_LOCK ? list[state] : PWM_STATE_FREE;
}

pwm_state_name_t pwm_state_back(pwm_state_name_t state) {
    const static pwm_state_name_t list [] = {
        PWM_STATE_CB,
        PWM_STATE_AB,
        PWM_STATE_AC,
        PWM_STATE_BC,
        PWM_STATE_BA,
        PWM_STATE_CA
    };
    const int idx = (int) state;
    return idx < PWM_STATE_LOCK ? list[state] : PWM_STATE_FREE;
}