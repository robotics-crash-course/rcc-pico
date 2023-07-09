#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    if(cyw43_arch_init())
    {
        cout << "CYW43 INIT failed!" << '\n';
    }
    cout << "init worked!" << '\n';

    cyw43_arch_gpio_put(0,1);

    //Init pushbutton
    rcc_init_pushbutton();

    //Init motors
    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 1000);
    MotorsOn(&motors);
    printf("AFTER MOTORS");

    //Init i2c    
    rcc_init_i2c();

    // Lidar
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    while(true)
    {   
        sleep_ms(100);
        if(!gpio_get(RCC_PUSHBUTTON))
        {
            cout << "PUSHBUTTON PRESSED!\n";
            MotorPower(&motors, 50, 50);
            // MotorPower(&motors, -50, 50); //use if rotating

            while(true){
                uint16_t distance = getFastReading(&lidar);

                if(distance <= 200){
                    MotorPower(&motors, 0, 0);
                }

                //use to stay a distance away from object
                // if(distance <= 200){
                //     MotorPower(&motors, -50, -50);
                // }
                // if(distance > 200){
                //     MotorPower(&motors, 50, 50);
                // }
            }
        }





    }

}