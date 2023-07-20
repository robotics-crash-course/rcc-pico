#include "rcc_stdlib.h"
using namespace std;

void update_angle(integral *ap, MPU6050 *imu) {
    if ((ap->current_time - ap->previous_time) >= ap->timestep) {
        ap->previous_time = ap->current_time;
        imu->update_pico();
        int angvel_z = imu->getAngVelZ(); //deg per sec
        ap->sum += angvel_z*(ap->timestep/1e6);
        //ap->sum += angvel_z*(0.002);
    }
}

int main() {
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    rcc_init_i2c(); //setup i2c
    rcc_init_pushbutton();

    Motor motors; //struct setup
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000); //setup
    MotorsOn(&motors); //enable PWM

    MPU6050 imu; //class setup
    imu.begin(i2c1); //adds to i2c1
    imu.calibrate(); //hold robot still
    sleep_ms(200);

    integral angular_position;
    angular_position.timestep = 2000;
    angular_position.current_time = time_us_64();
    angular_position.previous_time = time_us_64();
    angular_position.sum = 0;

    cyw43_arch_gpio_put(0, 0); //turns off led
    sleep_ms(100);
    cyw43_arch_gpio_put(0, 1); //turns on led

    while(true)
    {
        angular_position.current_time = time_us_64();

        update_angle(&angular_position, &imu);

        cout << angular_position.sum << '\n';
    }
}
