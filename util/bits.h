/* 作成者　terurin
 * 用途　基本的なbit操作
 */

#pragma once
#ifndef UTIL_BITS_HAEDER_GUARD
#define UTIL_BITS_HAEDER_GUARD

#include <stdint.h>
#include <stdbool.h>

//特定レジスタのビットを操作する。

static inline void bits_write_reg(volatile uint16_t* target, uint16_t mask, bool value) {
    if (value) {
        *target |= mask;
    } else {
        *target &= ~mask;
    }
}

static inline bool bits_read(uint16_t target, uint16_t number) {
    const uint16_t mask = 1u << number;
    return target&mask;
}

//[number+width-1,number]の領域の値を取得
static inline bool bits_read_width(uint16_t target, uint16_t width, uint16_t number) {
    const uint16_t width_mask = (1u << width) - 1;
    return width_mask & (target >> number);
}



#endif