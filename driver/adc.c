#include <p33FJ32MC204.h>
#include "adc.h"
#include <util/bits.h>
#include "config.h"
#include <util/ring.h>
#include <util/filter.h>
#include <math.h>
#include "pin.h"
// 仕様　VM,CA,CB,CCを設定
#define CHANNEL_SIZE (4)
#define CHANNEL_LENGTH (16)
static analog_id id_list[CHANNEL_SIZE];
//技術ノート
//TRC = 250 ns 
//FCY = 40 MHz -> TCY = 25 ns (1000/40 ns)
//Max Sampling Rate = 500 kHz(12bit), 1.1MHz(10bit)
//Min Sampling Period min = 2500 ns 
//仕様(データシート)より
//TAD min = 117.6 ns (12bit),76(10 bit) 
//TCONV = 12 TAD
//TSAMP = 3 TAD(but auto mode is not needed)

//計算
//4チャンネルが100kHz以上でで取り込めるように計算する
//TAD = FCY(=25 ns) * 4 = 100 ns = 0.100 us 
//TSAMP = TAD(=0.100 us) * 12 = 1.2 us -> 833.3 kHz
//つまり1channelあたり208.33kHzでサンプリングされる。

//内部バッファ
static uint16_t results[CHANNEL_SIZE][CHANNEL_LENGTH];
static uint16_t result_index;//次に書き込まれる場所

//FIRフィルタ構成データ
#define FIR_SIZE 3
static const uint16_t fir_filter[FIR_SIZE]={100,100,8};

//入力電圧係数
//Q-Format変換用のマクロ, xは被変換数,sはフォーマット形式
#define QCAST(x,s) (x)*((1<<s)-1) 
#define GAIN_FORMAT (4)
static const uint16_t vref_gain = QCAST(2.5,GAIN_FORMAT);
static const uint16_t vin_gains[CHANNEL_SIZE]={
    QCAST(12,GAIN_FORMAT),
    QCAST(1,GAIN_FORMAT),
    QCAST(1,GAIN_FORMAT),
    QCAST(1,GAIN_FORMAT)
};

void adc_init() {
    //定数定義
    const AD1CON1BITS con1 = {
        .ADON = false, //
        .ADSIDL = false, //running at idel
        .AD12B = false, //10bit mode
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
        .BUFS = false,//writing buffer 0~7
        .SMPI = 7, //interrupt at n+1 data collected
        .BUFM = true, //save half 
        .ALTS = false, //use only sample A
    };

    const AD1CON3BITS con3 = {
        .ADRC = false, //
        .SAMC = 0, // Auto Samping Rate(0~31), TAD
        .ADCS = 4 // TAD = (n+1)* TCY, n is 6 bit
    };
    int idx = 0;
    //管理領域初期化(制約　analog idの昇順に設定すること)
    id_list[idx++] = pin_cast_analog(PIN_VM);
    id_list[idx++] = pin_cast_analog(PIN_CA);
    id_list[idx++] = pin_cast_analog(PIN_CB);
    id_list[idx] = pin_cast_analog(PIN_CC);
    memset(results,0,sizeof(results));
    //memset(indexs,0,sizeof(indexs));
    result_index=0;
    
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

uint16_t adc_read_direct(adc_channel_id aid){
    return results[aid][result_index];
}


//データ列をコピーする。
uint16_t* adc_copy(adc_channel_id id,uint16_t* dest,size_t count){
    const uint16_t* result = results[id]; 
    uint16_t *it=dest;//destination buffer's iterator
    uint16_t start=(result_index-1+CHANNEL_LENGTH)%CHANNEL_LENGTH;
    uint16_t end =(result_index-1-count-CHANNEL_LENGTH)&CHANNEL_LENGTH;
    uint16_t pos;//ring buffer's position
    //ring_bufferなので折返しを考慮する。
    if (start>end){
        for (pos=start;pos<CHANNEL_LENGTH;pos++){
            *(it++)=result[pos];
        }
        start=0;
    }
    for (pos=start;pos<end;pos++){
         *(it++)=result[pos];
    }
    return dest;
}

uint16_t adc_read_raw(adc_channel_id id){
    uint16_t data[FIR_SIZE];
    adc_copy(id,data,FIR_SIZE);
    return filter_fir(data,fir_filter,FIR_SIZE);
}

uint16_t adc_read(adc_channel_id id){
    uint16_t rate = adc_read_raw(id);//Q16-Format
    uint16_t gain = vref_gain*vin_gains[id];//Q8-Format,Q4*Q4=Q8
    uint32_t raw =(uint32_t)rate*(uint32_t)gain;//Q24
    return raw>>14;//Q10, 24-14
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt() {
    //使うべきバッファを選択する
    volatile uint16_t* buf =  AD1CON2bits.BUFS?&ADC1BUF8:&ADC1BUF0;
    uint16_t idx;
    change_id ch=0;
    for (idx=0;idx<8;idx++){
        //store
        results[ch][result_index]=buf[idx];
        //update
        if ((++ch)==CHANNEL_SIZE){
            uint16_t next= result_index+1;
            ch=0;
            result_index=next<CHANNEL_LENGTH?next:0;
        }   
    }
    IFS0bits.AD1IF = false;
}