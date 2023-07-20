#include "rcc_stdlib.h"
using namespace std;

//define global constants 
#define WHEEL_RADIUS 0.04 //meters
#define COUNTS_PER_ROTATION 40 
#define IMU_FREQ 500 //Hz

int distanceToCounts(float distance){
    /*
    converts distance in meters [m] to encoder counts 
    takes in distance as a float
    returns integer
    */

    //converts distance to number of rotations
    //converts number of rotations to number of counts
    //round to whole counts
    //return counts as an integer
}

float hertz_to_s(int hertz) {
    /*
    converts sampling frequency (hertz) to seconds
    returns as a float
    */
}

int64_t hertz_to_us(int hertz){
    /*
    converts sampling frequency (hertz) to microseconds
    returns as integer
    */
}

int main() {
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    rcc_init_i2c(); //setup i2c
    rcc_init_pushbutton(); //setup button

    Motor motors; //struct setup
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000); //setup 
    MotorsOn(&motors); //enable PWM

    MPU6050 imu; //class setup
    imu.begin(i2c1); //adds to i2c1
    imu.calibrate(); //hold robot still
    sleep_ms(200); //wait a bit

    float angle_pos = 0.0; //angular position of robot
    float angvel_z; //imu returns a float
    float dt_s = hertz_to_s(IMU_FREQ); //seconds
    int dt_us = hertz_to_us(IMU_FREQ); //microseconds

    //track time since pico started
    int64_t current_time = time_us_64(); 
    int64_t previous_time = time_us_64(); 

    //because of ~physics~ may need to adjust
    //note: this is a positive rotation about the z-axis
    float deg_spin = 90.0;

    //use bool to stop until RESET
    bool stop = false; 

    //this blinks the led so we know it's done calibrating
    cyw43_arch_gpio_put(0, 0); //turns off led
    sleep_ms(100);
    cyw43_arch_gpio_put(0, 1); //turns on led

    while(true) 
    {
        //if button pressed, spin!
        if(!gpio_get(RCC_PUSHBUTTON)){
            //note: need to make sure spinning positively around z axis
            MotorPower(&motors, /*left*/, /*right*/); 
        }

        //update current time

        //if difference between current time and previous time is long enough
            //get IMU data
            //sum area of rectangles aka integrate (units are seconds)
            //make previous time the same as current time
        

        //check if we spun long enough
        if(angle_pos >= deg_spin){
            stop = true;
        }

        if(stop){
            MotorPower(&motors, 0,0); //stop until RESET
        }
    }
}