#include "driver.h"
#include <p33Fxxxx.h>

#include "uart.h"



int driver_init(){
    
    uart_init();
    
    return 0;
}

