#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();
    sleep_ms(100);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, true); //led on

    Motor motors; //struct setup
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000); //setup 
    MotorsOn(&motors); //enable PWM

    while(true)
    {   
        MotorPower(&motors, 100, 100); 
    }
}