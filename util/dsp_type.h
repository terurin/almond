/* 
 * 作成:terurin
 * 固定小数点型を定義
 */
#pragma once
#ifndef __UTIL_DSP_TYPE_HEADER_GURAD__
#define __UTIL_DSP_TYPE_HAERER_GUARD__

#include <stdint.h>
#include <stdbool.h>
/*補足
 * qXXYY_tの形式で表現されている型は
 * XXは整数bitでYYは小数点以下のbit数を意味する
 * 
 * qXX_tで表現されている型は
 * XXは小数点以下のbitを意味する
 */ 

//16bit with unsigned
typedef uint16_t q16_t;
typedef uint16_t q0016_t;
typedef uint16_t q0808_t;
typedef uint16_t q0610_t; 
//16bit with signed
typedef int16_t q15_t;
typedef int16_t q0015_t;
typedef int16_t q0708_t;

//32bit with signed
typedef int32_t q31_t;
typedef int32_t q0031_t;
typedef int32_t q2308_t;
typedef int32_t q1516_t;





#endif