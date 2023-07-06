#include "rcc_stdlib.h"
using namespace std;

void rcc_init_i2c_bus(void)
{
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
}

int main() {
    stdio_init_all();

    sleep_ms(1000);
    rcc_init_i2c_bus();
    
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
