#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();
    sleep_ms(1000);

    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 1000);
    MotorsOn(&motors);

    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }

    cyw43_arch_gpio_put(0, true);
    
    while(true)
    {   
        MotorPower(&motors, 100, 100);
    }

}