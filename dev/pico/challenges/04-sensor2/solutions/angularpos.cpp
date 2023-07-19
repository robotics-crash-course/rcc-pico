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

    MPU6050 imu; //struct
    imu.begin(i2c1); //adds to i2c1
    imu.calibrate(); //hold robot still

    int64_t timestep = 40000; //25 Hz in microseconds
    int64_t current_time = 0;
    int64_t previous_time = 0;
    float sum = 0;
    float angvel_z = 0.0;
    float deltat = 0.04; //25Hz in seconds
    previous_time = time_us_64();
    current_time = time_us_64();
    bool stop = false;
    bool moving = false;

    while(true) 
    {
        
        if(!gpio_get(RCC_PUSHBUTTON))
        {
            MotorPower(&motors, 60, -50);
        }

        //this block says wait 
        while((current_time - previous_time) < timestep){
            current_time = time_us_64(); //reset current time
        }

        previous_time = current_time;
        imu.update_pico(); 
        angvel_z = imu.getAngVelZ(); //deg per sec

        sum += angvel_z*deltat;

        if(sum >= (360.0*4.0) || sum <= -(360.0*4.0)){
            stop = true;
        }

        if(stop){
            MotorPower(&motors, 0,0);
        }
    }
}