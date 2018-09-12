#pragma once
#ifndef __CONTROL_HEADER_GUARD__
#define __CONTROL_HEADER_GUARD__

#include <util/tick.h>
#include <util/dsp_type.h>
#include <stdbool.h>
void free_process(void *obj);
void free_enter();
void lock_process(void* obj);
void lock_enter();
void duty_process(void* obj);
void duty_enter(q15_t duty);

struct force{
    stopwatch16_t stopwatch;
    tick32_t sum;
    tick32_t period;
    q15_t duty;
};
typedef struct force force_t;

void force_init(force_t* obj,tick32_t perioid,q15_t duty);
bool force_check_period(force_t* obj);//ステップの変更が必要か?
void force_process(void* force);
void force_enter(tick32_t perioid,q15_t duty);



#endif