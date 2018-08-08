#include "mid.h"
#include "led.h"
#include "hole.h"
#include "motor.h"


void mid_init(){
    led_init();
    hole_init();
    motor_init();
    
    
}