#include <p33FJ32MC204.h>
#include "adc.h"
#include <util/bits.h>
#include "config.h"
#include <util/ring.h>
#include <stdlib.h>
// 仕様　VM,CA,CB,CCを設定

#define CHANNEL_SIZE (4)
static analog_id id_list[CHANNEL_SIZE];
//技術ノート
//TRC = 250 ns 
//FCY = 40 MHz -> TCY = 25 ns (1000/40 ns)
//Max Sampling Rate = 500 kHz
//Min Sampling Period min = 2500 ns
//仕様(データシート)より
//TAD min = 117.6 ns
//TSAMP min= 3 TAD

//計算
//4チャンネルが100kHzで取り込めるように計算する
//TAD = FCY(=25 ns) * 50 = 500 ns = 0.5 us 
//TSAMP = TAD(=0.5 us) * 5 = 2.5 us -> 400 kHz

static int analog_id_compare(const void* a, const void* b) {
    return *(analog_id*) a - *(analog_id*) b;
}





void adc_init() {
    //定数定義
    const AD1CON1BITS con1 = {
        .ADON = false, //
        .ADSIDL = false, //running at idel
        .AD12B = true, //12bit mode
        .FORM = 0b10, //Q16-Format
        .SSRC = 0b111, //Auto Sampling by interrnal clock
        .SIMSAM = false, //Sequencail Convert Mode
        .ASAM = true, //Auto Convet Active
        .SAMP = false, //controlled by auto convert
        .DONE = false//clear
    };

    const AD1CON2BITS con2 = {
        .VCFG = 0b001, //ADREF+ = Vref+,ADREF- = AVSS
        .CHPS = 0b00, //scan ch0
        .BUFS = false, //
        .SMPI = 15, //interrupt at 16 data collected
        .BUFM = false, //save from 0 to 15 
        .ALTS = false, //use only sample A
    };

    const AD1CON3BITS con3 = {
        .ADRC = false, //
        .SAMC = 5, // Auto Samping Rate(0~31), TAD
        .ADCS = 49 // TAD = (n+1)* TCY, n is 6 bit
    };
    int idx = 0;
    //管理領域初期化
    id_list[idx++] = pin_cast_analog(PIN_VM);
    id_list[idx++] = pin_cast_analog(PIN_CA);
    id_list[idx++] = pin_cast_analog(PIN_CB);
    id_list[idx] = pin_cast_analog(PIN_CC);
    //ソートする
    qsort(id_list, CHANNEL_SIZE, sizeof (change_id), analog_id_compare);

    //入力設定
    AD1PCFGL = 0xffff; //すべてデジタルピンに変更する
    AD1CSSL = 0; //何もスキャンしない
    for (idx = 0; idx < CHANNEL_SIZE; idx++) {
        analog_id id = id_list[idx];
        bits_clear_reg(&AD1CSSL, id); //アナログピンに変更する
        bits_set_reg(&AD1CSSL, id); //自動的にスキャンさせる
    }

    //レジスタ書き込み
    AD1CON2bits = con2;
    AD1CON3bits = con3;
    AD1CON1bits = con1;

    //割り込み設定
    IFS0bits.AD1IF = false;
    IEC0bits.AD1IE = true;
    IPC3bits.AD1IP = ADC_PRI;
    //起動
    AD1CON1bits.ADON = true;
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt() {
    


    IFS0bits.AD1IF = false;
}