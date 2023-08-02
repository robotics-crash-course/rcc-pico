#include "rcc_stdlib.h"
using namespace std;

//define global constants 
#define IMU_FREQ 100 //Hz


float hertz_to_s(int hertz) {
    /*
    converts sampling frequency (hertz) to seconds
    returns as a float
    */
    return 1/(float)hertz;
}

int64_t hertz_to_us(int hertz){
    /*
    converts sampling frequency (hertz) to microseconds
    returns as integer
    */
    return 1e6/hertz; 
}

int main() {
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1);

    rcc_init_i2c(); 
    rcc_init_pushbutton(); 

    Motor motors; 
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000); 
    MotorsOn(&motors); 

    MPU6050 imu; 
    imu.begin(i2c1); 
    imu.calibrate(); 
    sleep_ms(200); 

    float velocity = 0.0;
    float accel_y = 0.0; //imu returns float
    float dt_s = hertz_to_s(IMU_FREQ); //seconds
    int dt_us = hertz_to_us(IMU_FREQ); //microseconds

    //track time since pico started
    int64_t current_time = time_us_64(); 
    int64_t previous_time = time_us_64(); 


    //this blinks the led so we know it's done calibrating
    cyw43_arch_gpio_put(0, 0); //turns off led
    sleep_ms(100);
    cyw43_arch_gpio_put(0, 1); //turns on led

    while(true) 
    {
        //update current time
        current_time = time_us_64(); 

        //if difference between current time and previous time is long enough
        if((current_time - previous_time) >= dt_us){ 
            //get IMU data
            imu.update_pico(); 
            accel_y = imu.getAccelY(); // m/s**2

            //sum area of rectangles aka integrate (units are seconds)
            velocity += accel_y*dt_s; 

            //make previous time the same as current time
            previous_time = current_time; 
        }

        cout <<"accel y "<< accel_y <<" || velocity [m/s] " << velocity <<'\n';

    }
}