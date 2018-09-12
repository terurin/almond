#include "tick.h"

tick16_t tick16_diff(tick16_t now, tick16_t last, tick16_t max) {
    if (now > last) {
        return now - last;
    } else {
        return max - last + now;
    }
}

tick32_t tick32_diff(tick32_t now, tick32_t last, tick32_t max) {
    if (now > last) {
        return now - last;
    } else {
        return max - last + now;
    }
}

msec16_t msec16(tick16_t tick, uint32_t tps) {
    return ((uint32_t) tick * 1000) / tps;
}

usec16_t usec16(tick16_t tick, uint32_t tps) {
    return ((uint32_t) tick * 1000000) / tps;
}

void stopwatch16_init(stopwatch16_t* obj, tick16_source_t source, uint16_t max) {
    if (!obj)return;
    obj->last = (obj->source = source)();
    obj->max = max;
}

tick16_t stopwatch16_count(stopwatch16_t *obj) {
    if (!obj)return;
    const tick16_t now = obj->source();
    const tick16_t diff =tick16_diff(now,obj->last,obj->max);
    obj->last=now;
    return diff;
}