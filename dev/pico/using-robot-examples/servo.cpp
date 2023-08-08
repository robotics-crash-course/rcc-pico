#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();
    sleep_ms(1000);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1);//led on

    Servo s3; //struct
    ServoInit(&s3, 18, false, 50); //setup on pin 18
    ServoOn(&s3); //enables PWM
    
    while(true)
    {   
        ServoPosition(&s3, 50); //faces forward
    }
}