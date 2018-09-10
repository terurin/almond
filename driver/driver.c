#include "driver.h"
#include <p33Fxxxx.h>
#include "clock.h"
#include "pwm.h"
#include "uart.h"
#include "cn.h"
#include "int.h"
#include "qei.h"
#include "timer.h"
#include "adc.h"
int driver_init(){
    //内蔵モジュールの初期化
    clock_init();
    timer23_init();
    int_init();
    change_init();
    uart_init();
    qei_init();
    pwm_init();
    adc_init();
    
    return 0;
}

