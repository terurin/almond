#include <mid/led.h>
#include <mid/mid.h>
#include <driver/pin.h>
#include <p33FJ32MC204.h>

#include <driver/driver.h>
#include <driver/pwm.h>

#include "driver/uart.h"
#include "driver/adc.h"

void test(void* obj){
    led_toggle(LED_A);
}

static inline void delay(uint16_t a){
    while (a>0){
        a--;
    }
}




int main(void) {
    driver_init();
    mid_init();
    
    for(;;){
        uart_putc('a');
    }
    
    return 0;
}
