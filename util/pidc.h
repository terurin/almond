// 作成　terurin
// 用途　pidのコントローラ

#ifndef UTIL_PIDC_HEADER_GUARD
#define UTIL_PIDC_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "qmath.h"


struct pidc{
    q0708_t p,i,d;//各種パラメータ
    int16_t sum;//累加制御偏差
    int16_t last;//前回の制御偏差
    int16_t sum_clip;//累加制御偏差の最大値(絶対値) 0はクリップしない
    int16_t result_clip;//最大出力(絶対値)
    int16_t count;//エラー検知(クリップされた回数)
};

typedef struct pidc pidc_t;
typedef struct pidc* pidc_ptr;
// P,I,D パラメータを設定する
void pidc_init(pidc_ptr obj,q0708_t p,q0708_t i,q0708_t d);
void pidc_clip(pidc_ptr obj,int16_t sum,int16_t result);//出力可能な範囲を設定する
int16_t pidc_process(pidc_ptr obj,int16_t now);

static inline uint16_t pidc_error(const pidc_ptr obj){
    return obj!=NULL?obj->count:0;
}

#endif