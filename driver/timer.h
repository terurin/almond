/* 作成　terurin
 * 用途　汎用タイマーの操作
 */


#pragma once
#ifndef DRIVER_TIMER_HEADER_GUARD
#define DRIVER_TIMER_HEADER_GUARD
#include <stdint.h>
//timer 1 制御用(16bit)
//timer 23 時間計測用(32bit)

typedef uint32_t clock_t;

void timer23_init();
clock_t timer23_clock();

clock_t clock_diff(clock_t now, clock_t last);



#endif