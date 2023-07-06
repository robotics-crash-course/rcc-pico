#include "rcc_stdlib.h"
using namespace std;

int main()
{   
    stdio_init_all();
    sleep_ms(100);
    // This example will use i2c1 on the default SDA and SCL pins (4, 5 on a Pico)
    rcc_init_i2c();

    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    while(true)
    {
        uint16_t dist = getFastReading(&lidar);
        printf("Distance (mm): %hu\n", dist);
        cout << lidar.timeoutOccurred() << '\n';
        sleep_ms(100);
    }
}