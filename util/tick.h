#pragma once
#include <stdint.h>

typedef uint16_t tick16_t;
typedef uint32_t tick32_t;
typedef uint32_t sec32_t;
typedef uint16_t msec16_t; //ミリ秒
typedef uint16_t usec16_t; //マイクロ秒

typedef tick16_t(*tick16_source_t)();

tick16_t tick16_diff(tick16_t now, tick16_t last, tick16_t max);
tick32_t tick32_diff(tick32_t now, tick32_t last, tick32_t max);

msec16_t msec16(tick16_t, uint32_t tps); //tick per sec
usec16_t usec16(tick16_t, uint32_t tps);

tick32_t tick_msec(msec16_t ,uint32_t tps);

struct stopwatch16 {
    tick16_source_t source;
    tick16_t max;
    tick16_t last;
};
typedef struct stopwatch16 stopwatch16_t;

void stopwatch16_init(stopwatch16_t* obj, tick16_source_t source, uint16_t max);
tick16_t stopwatch16_count(stopwatch16_t *obj);