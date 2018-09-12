#include <mid/led.h>
#include <mid/mid.h>
#include <driver/pin.h>
#include <p33FJ32MC204.h>

#include <driver/driver.h>
#include <driver/pwm.h>

#include "driver/uart.h"
#include "driver/adc.h"
#include "mid/hole.h"
#include "util/qmath.h"
#include "mid/motor.h"
#include "driver/timer.h"
#include "util/text.h"

static inline void delay(uint64_t a) {
    while (a > 0) {
        a--;
    }
}

static void init() {
    driver_init();
    mid_init();
    //motor_duty(QCAST(0.3, 15));
}

int main(void) {
    //volatile tick32_t tick;
    init();
    stopwatch16_t st;
    stopwatch16_init_timer1(&st);
    char buf[16];
    
    for (;;) {
        uint16_t d = stopwatch16_count(&st);
        text_hex16(buf,d);
        uart_putl(buf);
        delay(1000);
    }

    return 0;
}
