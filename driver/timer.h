/* 作成　terurin
 * 用途　汎用タイマーの操作
 */


#pragma once
#ifndef DRIVER_TIMER_HEADER_GUARD
#define DRIVER_TIMER_HEADER_GUARD

enum timer{
    TIMER1=0,
    TIMER2,
    TIMER3
};

typedef enum timer timer_t;


void timer1_init();

#endif