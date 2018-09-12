/* 作成　terurin
 * 用途　汎用タイマーの操作
 */


#pragma once
#ifndef DRIVER_TIMER_HEADER_GUARD
#define DRIVER_TIMER_HEADER_GUARD
#include <util/tick.h>

//時間変換用定数
extern const tick16_t timer1_max;
extern const uint32_t timer1_tps;
extern const tick32_t timer23_max;
extern const uint32_t timer23_tps;

//timer 1 制御用(16bit)
//timer 23 時間計測用(32bit)
void timer_init();
void timer1_init();
void timer23_init();
tick16_t timer1_tick();
tick32_t timer23_tick();
sec32_t timer32_seconds();

static inline tick16_t timer1_diff(tick16_t now,tick16_t last){
    return tick16_diff(now,last,timer1_max);
}

static inline tick32_t timer23_diff(tick32_t now,tick32_t last){
    return tick32_diff(now,last,timer23_max);
}

void stopwatch16_init_timer1(stopwatch16_t* obj);

#endif