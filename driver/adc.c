#include <p33FJ32MC204.h>
#include "adc.h"
#include <util/bits.h>
// 仕様　VM,CA,CB,CCを
static analog_id vm,ca,cb,cc;

void adc_init(){
    const AD1CON1BITS con1 ={
        .ADON=false,//
        .ADSIDL=false,//running at idel
        .AD12B=true,//12bit mode
        .FORM=0b10,//Q16-Format
        .SSRC=0b111,//d
        
    };
    
    const AD1CON2BITS con2={
        
    };
    
    const AD1CON3BITS con3={
        
    };
    
    //管理領域初期化
    vm = pin_cast_analog(PIN_VM);
    ca = pin_cast_analog(PIN_CA);
    cb = pin_cast_analog(PIN_CB);
    cc = pin_cast_analog(PIN_CC);
    //入力設定
    AD1PCFGL=0xffff;//すべてデジタルピンに変更する
    bits_clear_reg(&AD1CSSL,vm);
    bits_clear_reg(&AD1CSSL,ca);
    bits_clear_reg(&AD1CSSL,cb);
    bits_clear_reg(&AD1CSSL,cc);
    
    
    //自動スキャン設定
    AD1CSSL=0;
    bits_set_reg(&AD1CSSL,vm);
    bits_set_reg(&AD1CSSL,ca);
    bits_set_reg(&AD1CSSL,cb);
    bits_set_reg(&AD1CSSL,cc);
}