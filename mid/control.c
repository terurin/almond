#include "control.h"
#include <driver/pwm.h>
#include "hole.h"
#include "driver/timer.h"
#include <util/qmath.h>
//ホールセンサの状態とPWM出力状態の対応表
//前進
const static pwm_state_name_t table_front[8] = {
    PWM_STATE_CA, //A->AB
    PWM_STATE_CB, //AB->CB
    PWM_STATE_AB, //B->AB
    PWM_STATE_AC, //BC->AC
    PWM_STATE_BC, //C->BC
    PWM_STATE_BA, //CA->BA
    PWM_STATE_FREE//null->free
};

//後退
const static pwm_state_name_t table_back[8] = {
    PWM_STATE_BA, //A->BA
    PWM_STATE_CA, //AB->CA
    PWM_STATE_CB, //B->CB
    PWM_STATE_AB, //BC->AB
    PWM_STATE_AC, //C->AC
    PWM_STATE_BC, //CA->BC
    PWM_STATE_FREE//null->free
};

void free_process(void* obj) {
    pwm_write_free();
}

void free_enter() {
    pwm_event(free_process, NULL);
}

void lock_process(void* obj) {
    pwm_write_lock();
}

void lock_enter() {
    pwm_event(lock_process, NULL);
}

void duty_process(void* obj) {
    const hole_t hole = hole_sense();
    const q15_t x = (obj != NULL) ? *(int16_t*) obj : 0;
    const pwm_state_name_t next = (x >= 0) ? table_front[hole] : table_back[hole];
    const q16_t abs = abs15_s(x);
    pwm_write(next, abs);
}

void duty_enter(q15_t duty) {
    static q15_t instance;
    instance = duty; //stackから静的領域にコピーする
    pwm_event(duty_process, &duty);
}

void force_init(force_t* obj, tick32_t perioid, q15_t duty) {
    if (!obj)return;
    stopwatch16_init_timer1(&obj->stopwatch);
    obj->sum = 0;
    obj->period = perioid;
    obj->duty = duty;
}

bool force_check_period(force_t* obj) {
    tick32_t sum = stopwatch16_count(&obj->stopwatch) + obj->sum;
    bool result = sum >= obj->period;
    obj->sum = result ? 0 : sum; //更新
    return result;
}

void force_process(void *obj) {
    force_t* p = (force_t*) obj;
    const q16_t abs = abs15_s(p->duty);
    if (force_check_period(p)) {
        const pwm_state_name_t next = p->duty >= 0 ? pwm_state_next(pwm_state()) : pwm_state_back(pwm_state());
        pwm_write(next, abs);
    } else {
        pwm_write(pwm_state(), abs);
    }
}

void force_enter(tick32_t perioid, q15_t duty) {
    static force_t instance;
    force_init(&instance, perioid, duty);
    pwm_event(force_process, &instance);
}