/* 作成　terurin
 * 用途　汎用タイマーの操作
 */


#pragma once
#ifndef DRIVER_TIMER_HEADER_GUARD
#define DRIVER_TIMER_HEADER_GUARD
#include <stdint.h>

typedef uint32_t tick_t;
typedef uint16_t sec_t;//sec
typedef uint16_t msec_t;//mill sec
typedef uint16_t usec_t;//micro sec
typedef uint16_t minutes_t;//minutes(tickからは取得できない)
//時間変換関数
extern uint32_t timer_fcy;
extern const tick_t tick_max;
static inline sec_t tick_cast_sec(tick_t t){
    return t/timer_fcy;
}

static inline tick_t tick_sec(sec_t t){
    return t*timer_fcy;
}

static inline sec_t tick_cast_msec(tick_t t){
    return (1000*t)/timer_fcy;
}

static inline tick_t tick_msec(msec_t t){
    return t*(timer_fcy/1000);
}

static inline sec_t tick_cast_usec(tick_t t){
    return (1000000*t)/timer_fcy;
}

static inline tick_t tick_usec(usec_t t){
    return t*(timer_fcy/1000000);
}

static inline tick_t tick_add(tick_t a ,tick_t b){
    return a+b;
}

static inline tick_t tick_diff(tick_t now,tick_t last){
    //overflow check 
    if (now>last){
        return now-last;
    }else{
        return tick_max-last+now;
    }
}



//timer 1 制御用(16bit)
//timer 23 時間計測用(32bit)
//typedef void (*timer_handler_t)(void *obj);
//void timer1_init();


void timer23_init();
tick_t timer23_tick();




#endif