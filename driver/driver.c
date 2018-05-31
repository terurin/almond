#include "driver.h"
#include <p33Fxxxx.h>
#include "clock.h"
#include "pwm.h"
#include "uart.h"
#include "cn.h"
#include "int.h"


int driver_init(){
    clock_init();
    pwm_init();
    int_init();
    change_init();
    
    uart_init();
    qei_init();
    return 0;
}

