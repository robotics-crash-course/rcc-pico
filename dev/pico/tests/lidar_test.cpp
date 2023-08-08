#include "rcc_stdlib.h"
using namespace std;

int main()
{   
    stdio_init_all();
    sleep_ms(100);
    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }
    
    cyw43_arch_gpio_put(0, 1); //turns on led
    // This example will use i2c1 on the default SDA and SCL pins (4, 5 on a Pico)
    rcc_init_i2c();

    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    while(true)
    {
        // uint16_t dist = getFastReading(&lidar);
        uint16_t dist = lidar.readRangeSingleMillimeters();
        printf("Distance (mm): %hu\n", dist);
        // sleep_ms(10);
    }
}