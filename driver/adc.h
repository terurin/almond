// 作成者　terurin
// 用途 ADC

#ifndef __DRIVER_ADC_HEADER_GUARD__
#define __DRIVER_ADC_HEADER_GUARD__

enum adc_channel{
    ADC_CHANNEL_VM=0,
    ADC_CHANNEL_CA,
    ADC_CHANNEL_CB,
    ADC_CHANNEL_CC,
    ADC_CHANNEL_END//終端処理用
};

typedef enum adc_channel adc_channel_id;

void adc_init();

uint16_t adc_read_raw_now(adc_channel_id);//最新の状態のみ取得

uint16_t adc_read_raw(adc_channel_id);//フィルターを書けられた値
uint32_t adc_read(adc_channel_id);//フィルターを書けられた値






#endif