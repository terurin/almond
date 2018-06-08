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

static inline void bits_write(uint16_t* target,uint16_t number,bool value){
    const uint16_t mask=1u<<number;
    uint16_t tmp=*target;
    *target=value?tmp|mask:tmp&~mask;
}

static inline void bits_set_reg(volatile uint16_t* target,uint16_t number){
    const uint16_t mask=1u<<number;
    *target|=mask;
}

static inline void bits_clear_reg(volatile uint16_t* target,uint16_t number){
    const uint16_t mask=1u<<number;
    *target&=~mask;
}

static inline void bits2_set_reg(volatile uint16_t* h,volatile uint16_t *l,uint16_t number){
    uint16_t mask = 1u<<(number&0x0f);
    volatile uint16_t *target =(number&0x10)?h:l;
    *target |= mask;
}

static inline void bits2_clr_reg(volatile uint16_t* h,volatile uint16_t *l,uint16_t number){
    uint16_t mask = 1u<<(number&0x0f);
    volatile uint16_t *target =(number&0x10)?h:l;
    *target &= ~mask;
}

static inline void bits2_write_reg(volatile uint16_t* h,volatile uint16_t *l,uint16_t number,bool value){
    uint16_t mask = 1u<<(number&0x0f);
    volatile uint16_t *target =(number&0x10)?h:l;
    *target= value?*target|mask:*target&~mask;
}



#endif