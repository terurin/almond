// 作成　terurin
// 用途　pidのコントローラ

#ifndef UTIL_PIDC_HEADER_GUARD
#define UTIL_PIDC_HEADER_GUARD

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#ifndef Q0708
#define Q0708
typedef int16_t q0708_t;
static inline q0708_t q0708(float x){
    return x>=0?(q0708_t)(x*255.0f):(q0708_t)(x*256.0f);
}
static inline float q0708_cast(q0708_t x){
    return x>=0?(float)x/255.0f:(float)x/256.0f;
}
#endif

#ifndef Q2308
#define Q2308
typedef int32_t q2308_t;
static inline q2308_t q2308(float x){
    return x>=0?(q2308_t)(x*255.0f):(q2308_t)(x*256.0f);
}
static inline float q2308_cast(q2308_t x){
    return x>=0?(float)x/255.0f:(float)x/256.0f;
}
#endif

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