/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rcc_stdlib.h"
using namespace std;

int main() {
    stdio_init_all();
    
    sleep_ms(1000);
    
    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }
    
    cyw43_arch_gpio_put(0, 1); //turns on led

    // This example will use i2c1 on the default SDA and SCL pins (4, 5 on a Pico)
    rcc_init_i2c();

    printf("Resetting MPU6050\n");

    // mpu6050_reset();
    MPU6050 imu;
    imu.begin(i2c1);
    printf("Hello, MPU6050! Reading raw data from registers...\n");

    int16_t acceleration[3], gyro[3], temp;
    imu.calibrate();
    while (1) {
        imu.update_pico();
        printf("Accel: %f, %f, %f\n", imu.getAccelX(), imu.getAccelY(), imu.getAngVelZ());
        sleep_ms(100);
    }

    return 0;
}
