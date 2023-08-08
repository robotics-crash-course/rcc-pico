#include "rcc_stdlib.h"
using namespace std;

//since Motor struct is declared in the scope of main, 
//need to pass a pointer in our functions
//functions accept a pointer to Motor, calls it motorPointer

void drive_forwards(Motor* motorPointer){
    int base_power = 50;
    int scale_factor = 1.2;
    MotorPower(motorPointer, static_cast<int>(base_power*scale_factor), base_power);
}

void drive_backwards(Motor* motorPointer){
    int base_power = 50;
    int scale_factor = 1.2;
    MotorPower(motorPointer, static_cast<int>(-1*base_power*scale_factor), -1*base_power);
}

void stop(Motor* motorPointer){
    MotorPower(motorPointer, 0, 0);
}

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    Motor motors; //keeping struct in main's scope
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
            drive_forwards(&motors); 
        }

        //goes forward 200 counts
        if(left_count >= 200 && right_count >= 200){
            drive_backwards(&motors);
        }

        //goes backwards 200 more counts
        if(left_count >=400 && right_count >= 400){
            stop(&motors);
        }
    }

}