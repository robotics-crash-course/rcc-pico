#include "rcc_stdlib.h"
using namespace std;

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

    int64_t timestep = 2000; //500 Hz in microseconds
    // int64_t timestep = 2500; //400 Hz in microseconds
    // int64_t timestep = 10000; //100Hz
    // int64_t timestep = 1000; //1kHz
    int64_t current_time = time_us_64();
    int64_t previous_time = time_us_64();
    float sum = 0;
    float angvel_z = 0.0;
    float deltat = 0.002; // 500 Hz in seconds
    // float deltat = 0.0025; // 400 Hz in seconds
    // float deltat = 0.01; //100Hz
    // float deltat = 0.001; //1kHz
    bool stop = false;

    cyw43_arch_gpio_put(0, 0); //turns off led
    sleep_ms(100);
    cyw43_arch_gpio_put(0, 1); //turns on led

    while(true) 
    {
        if(!stop){
            MotorPower(&motors, -75, 60);
        }

        current_time = time_us_64(); //get current time

        if((current_time - previous_time) >= timestep){ //int math
            previous_time = current_time; //reset previous time
            imu.update_pico(); 
            angvel_z = imu.getAngVelZ(); //deg per sec
            sum += angvel_z*deltat; //float math
        }

        cout << sum << '\n';

        if(sum >= (360.0)){
            stop = true;
        }

        if(stop){
            MotorPower(&motors, 0,0);
        }
    }
}