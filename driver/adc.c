#include <p33FJ32MC204.h>
#include "adc.h"
#include <util/bits.h>
#include "config.h"
#include <util/ring.h>
#include <util/filter.h>
#include <util/qmath.h>
#include <math.h>
#include "pin.h"
// 仕様　VM,CA,CB,CCを設定
#define CHANNEL_SIZE (4) //最適化のため、変更禁止
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
static uint16_t results[CHANNEL_SIZE*CHANNEL_LENGTH];
static uint16_t result_index;//次に書き込まれる場所
static const size_t result_total = CHANNEL_LENGTH*CHANNEL_SIZE;
//FIRフィルタ構成データ
#define FIR_SIZE 3
static const uint16_t fir_filter[FIR_SIZE]={100,100,8};

//入力電圧係数


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
//低速
uint16_t adc_read_direct(adc_channel_id id){
    size_t newest = result_index!=0?(result_index-CHANNEL_SIZE+id):result_total-CHANNEL_SIZE+id;
    return results[newest];    
}

//データ列をコピーする。
uint16_t* adc_copy(adc_channel_id id,uint16_t* dest,size_t count){
    const size_t newest = result_index!=0?(result_index-CHANNEL_SIZE+id):result_total-CHANNEL_SIZE+id;
    int16_t pos=newest,next;
    size_t idx;
    for (idx=0;idx<count;idx++){
        dest[idx]=results[pos];
        next = pos-CHANNEL_SIZE;
        pos = next >=0?next:result_total-CHANNEL_SIZE+id;
    }
    return dest;
}

uint16_t adc_read_raw(adc_channel_id id){
    uint16_t data[FIR_SIZE];
    adc_copy(id,data,FIR_SIZE);
    return filter_fir(data,fir_filter,FIR_SIZE);
}

q0610_t adc_read(adc_channel_id id){
    uint16_t rate = adc_read_raw(id);//Q16-Format
    uint16_t gain = vref_gain*vin_gains[id];//Q8-Format,Q4*Q4=Q8
    uint32_t raw =(uint32_t)rate*(uint32_t)gain;//Q24
    return raw>>14;//Q10, 24-14
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt() {
    //使うべきバッファを選択する
    volatile uint16_t* buf =  AD1CON2bits.BUFS?&ADC1BUF8:&ADC1BUF0;
    memcpy(&results[result_index],(void*)buf,sizeof(uint16_t)*8);
    size_t next =result_index+8;
    result_index = next >result_total?0:next;
    IFS0bits.AD1IF = false;
}