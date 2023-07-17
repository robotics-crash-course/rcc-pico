#include "rcc_stdlib.h"
using namespace std;

void blink_led(int time){
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //blinks LED
    sleep_ms(time);
}

bool searching(Motor* motorPointer, VL53L0X* lidarPointer){
    uint16_t distance = getFastReading(lidarPointer); //check lidar
    MotorPower(motorPointer, -50, 50); //spin~~

    if(distance <= 200){
        return true;
    }
    else{
        return false;
    }
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
    printf("AFTER MOTORS");

    rcc_init_i2c();
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    bool start_search = false;
    bool stop = false;
    bool blinking = false;

    while(true){   

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            ServoPosition(&s3, 50); //look forwards
            start_search = true;
            stop = false;
            blinking = false;
        }

        if(start_search){
            if(searching(&motors, &lidar)){ //returns true when object detected
                stop = true;
            }; 
        }

        if(stop){
            MotorPower(&motors, 0, 0); //stop
            blinking = true;
        }

        if(blinking){
            blink_led(300);
        }
    }

}