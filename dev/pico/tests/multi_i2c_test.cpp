#include "rcc_stdlib.h"
using namespace std;

int main() {
    stdio_init_all();

    sleep_ms(1000);
    rcc_init_i2c();

    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }
    
    cyw43_arch_gpio_put(0, 1); //turns on led
    
    //Instantiate IMU and Lidar classes
    MPU6050 imu;
    VL53L0X lidar;
    imu.begin(i2c1);
    rcc_init_lidar(&lidar);
    printf("Both Sensors initialized!, Calibrating IMU");
    imu.calibrate();

    while (1) {
        imu.update_pico();

        uint16_t dist = getFastReading(&lidar);
        printf("Accel: %f, %f, %f Distance (mm): %hu\n", imu.getAccelX(), imu.getAccelY(), imu.getAccelZ(), dist);
        sleep_ms(100);
    }

    return 0;
}
