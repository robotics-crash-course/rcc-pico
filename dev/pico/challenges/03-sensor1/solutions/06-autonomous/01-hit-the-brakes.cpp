#include "rcc_stdlib.h"
using namespace std;

void blink_led(int time){
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //blinks LED
    sleep_ms(time);
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

    bool forwards = false;
    bool stop = false;
    bool blinking = false;
    uint16_t distance;

    while(true){   

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            blinking = false;
            ServoPosition(&s3, 50); //look forwards
            MotorPower(&motors, 60, 50); //go forwards
        }
        //in while loop update!
        distance = getFastReading(&lidar); //check lidar

        if(distance <= 200){
            MotorPower(&motors, 0, 0); //stop
            blinking = true;
        }

        if(blinking){
            blink_led(300);
        }
    }

}