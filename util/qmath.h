//作成者 terurin
//用途　固定小数点型の演算

#ifndef UTIL_QMATH_HEADER_GUARD
#define UTIL_QMATH_HEADER_GUARD

#include <stdint.h>
#include "dsp_type.h"

static inline q0708_t q0708_f(float x){
    return x>=0?(q0708_t)(x*255.0f):(q0708_t)(x*256.0f);
}
static inline float q0708_cast(q0708_t x){
    return x>=0?(float)x/255.0f:(float)x/256.0f;
}

static inline q15_t q15_f(float x){
    return x>=0?(q0708_t)(x*INT16_MAX):(q0708_t)(x*-INT16_MIN);
}




static inline q2308_t q2308_f(float x){
    return x>=0?(q2308_t)(x*255.0f):(q2308_t)(x*256.0f);
}
static inline float q2308_cast(q2308_t x){
    return x>=0?(float)x/255.0f:(float)x/256.0f;
}

//安全な絶対値の取得関数
static inline int16_t qabs16(int16_t x){
    if (x>0)return x;
    else if(x!=INT16_MIN)return -x;
    else return INT16_MAX;
}


//Q-Format変換用のマクロ, xは被変換数,sはフォーマット形式
#define QCAST(x,s) (x)*((1ul<<s)-1) 




#endif