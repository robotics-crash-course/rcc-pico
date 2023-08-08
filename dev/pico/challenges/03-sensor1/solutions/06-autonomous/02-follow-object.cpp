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

    rcc_init_i2c();
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    bool follow = false;
    uint16_t distance;

    while(true){   

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            follow = true;
        }

        if(follow){
            ServoPosition(&s3, 50); //look forwards
            distance = getFastReading(&lidar); //check lidar
            sleep_ms(100);

            if(distance < 200){
                MotorPower(&motors, -60, -50); //go backwards
            }
            else{
                MotorPower(&motors, 60, 50); //go forwards
            }
            //note: this is a little janky but it works lol
        }
    }

}