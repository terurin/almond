#include <mid/led.h>
#include <p33FJ32MC204.h>
int main(void) {
    
    led_init();
    led_on(LED_A);
    for(;;);


   
    return 0;
}
