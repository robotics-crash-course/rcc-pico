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

    rcc_init_i2c();
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    bool searching = false;
    bool stop = false;
    uint16_t distance;

    while(true){   

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            searching = true;
            stop = false;
        }

        if(searching){
            ServoPosition(&s3, 50); //look forwards
            distance = getFastReading(&lidar); //check lidar
            MotorPower(&motors, -50, 50); //spin~~

            if(distance <= 200){
                stop = true;
            }
        }
        if(stop){
            MotorPower(&motors, 0, 0); //stop
        }
    }

}