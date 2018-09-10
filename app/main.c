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

static inline void delay(uint64_t a) {
    while (a > 0) {
        a--;
    }
}

static void init() {
    driver_init();
    mid_init();
    motor_duty(QCAST(0.3, 15));
}

int main(void) {
    init();

    for (;;) {
        char c = hole_sense_raw() + '0';
        uart_putc(c);
    }

    return 0;
}
