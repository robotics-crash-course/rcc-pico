#include "rcc_stdlib.h"
using namespace std;

int main()
{   
    stdio_init_all();
    sleep_ms(100);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    rcc_init_i2c(); //setup pico i2c
    VL53L0X lidar; //class 
    rcc_init_lidar(&lidar); //setup lidar

    while(true)
    {
        uint16_t dist = getFastReading(&lidar);
        cout << "distance: " << dist << "\n";
        sleep_ms(100);
    }
}