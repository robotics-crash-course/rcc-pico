#include "rcc_stdlib.h"
using namespace std;

//DISCLAIMER: DOES NOT WORK YET -jc

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 1000);
    MotorsOn(&motors);

    rcc_init_i2c();

    Left_Odom left;
    Right_Odom right;

    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    int positions[10]; //array for servo positions

    for (int i = 0; i < 10; ++i) {
        positions[i] = i*10;  //filling array
    }

    int i = 0;
    uint16_t distance;
    bool looking = false;
    bool rotating = false;
    bool located = false;
    bool stop = false;


    while(true) {   

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            looking = true;
            stop = false;
        }

        distance = getFastReading(&lidar); //want to always check

        if(looking){
            if(distance >= 200){
                ServoPosition(&s3, positions[i]);
                i++;
                if (i >= 9){
                    i = 0;
                }
            }
            else{ //distance less than 200
                looking = false;
                rotating = true;
            }
        }

        if(rotating){
            if (i <= 5){  //object was on left
                MotorPower(&motors, 50, -50); //turn left
            }
            else{ //object was on right
                MotorPower(&motors, -50, 50); //turn right
            }

            if (distance <= 200){
                rotating = false;
                located = true;
            }
        }

        if(located){
            MotorPower(&motors, 60, 50); //go forward
            if (distance <= 20){
                stop = true;
            }
        }

        if(stop){
            MotorPower(&motors, 0, 0);
        }
    }

}