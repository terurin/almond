#include <mid/led.h>
#include <p33FJ32MC204.h>

#include <driver/driver.h>
#include <driver/pwm.h>

void test(void* obj){
    led_toggle(LED_A);
}

int main(void) {
    driver_init();
    
    pwm_rate_write_all(Q15(0.9));
    pwm_event(test,NULL);
    
    for(;;);
    return 0;
}
