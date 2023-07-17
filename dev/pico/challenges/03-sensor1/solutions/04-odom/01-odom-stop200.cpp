#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);

    Left_Odom left;
    Right_Odom right;

    int left_count = 0;
    int right_count = 0;

    while(true)
    {   
        left_count = left.getCount();
        right_count = right.getCount();

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            MotorPower(&motors, 60, 60); //use something above the deadband
        }

        if(left_count >= 200 && right_count >= 200){
            MotorPower(&motors, 0, 0); //stop motors
        }

    }

}