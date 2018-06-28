// 作成者　terurin
// 用途 ADC

#ifndef __DRIVER_ADC_HEADER_GUARD__
#define __DRIVER_ADC_HEADER_GUARD__

#include <stddef.h>
#include <util/dsp_type.h>

enum adc_channel{
    ADC_CHANNEL_VM=0,
    ADC_CHANNEL_CA,
    ADC_CHANNEL_CB,
    ADC_CHANNEL_CC,
    ADC_CHANNEL_END//終端処理用
};

typedef enum adc_channel adc_channel_id;

void adc_init();

//最新の生な値を返す
uint16_t adc_read_direct(adc_channel_id);
//ring bufferの値を配列に変換して格納
//制約 count < CHANNEL_LENGTHを満たすこと
uint16_t* adc_copy(adc_channel_id id,uint16_t* dest,size_t count);
//FIRフィルターを掛けられた値(100kHz)
uint16_t adc_read_raw(adc_channel_id);//フィルターを書けられた値
//FIRフィルターを掛けた電圧(Q10-Format)
q0610_t adc_read(adc_channel_id);//フィルターを書けられた値





#endif