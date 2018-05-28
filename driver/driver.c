#include "driver.h"
#include <p33Fxxxx.h>
#include "clock.h"
#include "pwm.h"
#include "uart.h"



int driver_init(){
    clock_init();
    pwm_init();
    
    
    uart_init();
    
    return 0;
}

