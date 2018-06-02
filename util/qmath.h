//作成者 terurin
//用途　固定小数点型の演算

#ifndef UTIL_QMATH_HEADER_GUARD
#define UTIL_QMATH_HEADER_GUARD

#include <stdint.h>

typedef int16_t q0708_t;
static inline q0708_t q0708(float x){
    return x>=0?(q0708_t)(x*255.0f):(q0708_t)(x*256.0f);
}
static inline float q0708_cast(q0708_t x){
    return x>=0?(float)x/255.0f:(float)x/256.0f;
}

typedef int32_t q2308_t;
static inline q2308_t q2308(float x){
    return x>=0?(q2308_t)(x*255.0f):(q2308_t)(x*256.0f);
}
static inline float q2308_cast(q2308_t x){
    return x>=0?(float)x/255.0f:(float)x/256.0f;
}




#endif