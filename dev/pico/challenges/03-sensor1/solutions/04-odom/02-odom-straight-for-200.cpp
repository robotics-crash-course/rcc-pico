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
    printf("AFTER MOTORS");

    Left_Odom left;
    Right_Odom right;
    printf("AFTER ODOM");

    int left_count = 0;
    int right_count = 0;
    float scale_factor = 1.2; //can vary greatly between motors
    int base_power = 50;

    while(true)
    {   
        left_count = left.getCount();
        right_count = right.getCount();

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            //note: MotorPower accepts integers so its rounded to the nearest integer...
            //can force it to be an integer first using:
            MotorPower(&motors, static_cast<int>(base_power*scale_factor), base_power);

        }

        if(left_count >= 200 && right_count >= 200){
            MotorPower(&motors, 0, 0); //stop
        }

    }

}