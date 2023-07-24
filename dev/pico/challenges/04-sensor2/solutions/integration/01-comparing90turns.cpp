#include "rcc_stdlib.h"
using namespace std;

#define IMU_FREQ 500 //Hz

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

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);

    Left_Odom left;
    Right_Odom right;

    rcc_init_i2c();
    MPU6050 imu; 
    imu.begin(i2c1); 
    imu.calibrate(); 
    sleep_ms(500); 

    int left_count = 0;
    int right_count = 0;
    int rot90deg = 65; //hardcoded value

    float angle_pos = 0.0; 
    float angvel_z = 0.0; 
    float deg_spin = -90.0; 
    float dt_s = hertz_to_s(IMU_FREQ); //seconds
    int dt_us = hertz_to_us(IMU_FREQ); //microseconds

    int64_t current_time = time_us_64(); 
    int64_t previous_time = time_us_64();

    bool stop = false;

    //this blinks the led so we know it's done calibrating
    cyw43_arch_gpio_put(0, 0); //turns off led
    sleep_ms(100);
    cyw43_arch_gpio_put(0, 1); //turns on led

    while(true)
    {   
        cout << "yo" << '\n';
        if(!gpio_get(RCC_PUSHBUTTON))
        {
            MotorPower(&motors, 50, 0); //spins left wheel only
        }

        //for purposes of the solution: 
        //going to check both and print which saw it ~~first~~

        //this option is for using encoder
        left_count = left.getCount();
        if(left_count >= rot90deg){
            stop = true;
            cout << "encoder saw it first" << '\n';
        }

        //this option for using integrator
        current_time = time_us_64(); 

        if((current_time - previous_time) >= dt_us){ 
            imu.update_pico(); 
            angvel_z = imu.getAngVelZ(); //deg per sec
            angle_pos += angvel_z*dt_s; 
            previous_time = current_time; 
        }

        if(angle_pos <= deg_spin){ //less than neg 90
            stop = true;
            cout << "imu saw it first" << '\n';
        }

        if(stop){
            MotorPower(&motors, 0,0); //stop until RESET
        }

    }

}