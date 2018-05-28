#include <mid/led.h>
#include <p33FJ32MC204.h>

#include "driver/driver.h"
#include "driver/pwm.h"
int main(void) {
    driver_init();
    pwm_duty(Q15(0.9));
    for(;;);


   
    return 0;
}
