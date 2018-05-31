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







#endif