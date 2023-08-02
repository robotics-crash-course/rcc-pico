#include "rcc_stdlib.h"
using namespace std;

//define global constants 
#define WHEEL_RADIUS 0.04 //meters
#define COUNTS_PER_ROTATION 40 

int distanceToCounts(float distance){
    /*
    converts distance in meters [m] to encoder counts 
    takes in distance as a float
    returns integer
    */
    float rotations = distance/(2*3.14*WHEEL_RADIUS);
    float counts_f = rotations*COUNTS_PER_ROTATION;
    int counts = (int)counts_f;
    return counts;
}

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

    int counts_need = distanceToCounts(1.0); //1 meter

    while(true)
    {   
        left_count = left.getCount();
        right_count = right.getCount();

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            MotorPower(&motors, 60, 60); //can go straight if you have those values
        }

        if(left_count >= counts_need && right_count >= counts_need){
            MotorPower(&motors, 0, 0); //stop motors
        }

    }

}