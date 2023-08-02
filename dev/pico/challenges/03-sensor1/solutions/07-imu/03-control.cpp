#include "rcc_stdlib.h"
using namespace std;

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

    MPU6050 imu; //struct
    imu.begin(i2c1); //adds to i2c1
    imu.calibrate(); //hold robot still

    int base_power = 50;
    int power_boost = 0;
    float desired_angvel_z = 150.0;
    float angvel_z = 0.0;
    float error = 0.0;
    float kp = 1.2;
    float tolerance = 5.0; 


    bool spinning = false;

    while(true) 
    {
        if(!gpio_get(RCC_PUSHBUTTON)){
            spinning = true;
        }
        
        if(spinning){
            imu.update_pico(); //updates data
            angvel_z = imu.getAngVelZ(); //gets data

            //calculate error 
            error = desired_angvel_z - angvel_z;
            //make proportional power boost
            power_boost = static_cast<int>(kp*error);
            //set motor power
            MotorPower(&motors, -(base_power+power_boost), (base_power+power_boost));

            if (error < tolerance){
                cyw43_arch_gpio_put(0, 0); //led off
            }
            else{
                cyw43_arch_gpio_put(0, 1); //led on
            }
        }
    }
}
