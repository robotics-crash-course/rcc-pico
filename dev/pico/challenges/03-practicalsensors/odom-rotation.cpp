#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    if(cyw43_arch_init())
    {
        cout << "CYW43 INIT failed!" << '\n';
    }
    cout << "init worked!" << '\n';

    cyw43_arch_gpio_put(0,1);

    //Init potentiometer and pushbutton
    rcc_init_potentiometer();
    rcc_init_pushbutton();

    //Init servo
    Servo s1;
    Servo s2;
    ServoInit(&s1, 16, false, 50);
    ServoInit(&s2, 17, false, 50);
    ServoOn(&s1);
    ServoOn(&s2);
    printf("AFTER SERVO");


    //Init motors
    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 1000);
    MotorsOn(&motors);
    printf("AFTER MOTORS");

    //INit i2c and mpu6050    
    rcc_init_i2c();
    MPU6050 imu;
    imu.begin(i2c1);
    imu.calibrate();
    float accelx, accely, angvelz;
    printf("AFTER IMU");

    // //Odom
    Left_Odom left;
    Right_Odom right;
    printf("AFTER ODOM");

    // Lidar
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    int left_count = 0;
    int right_count = 0;
    // float scale_factor = 2.0;
    int rot90deg = 65;

    while(true)
    {   
        left_count = left.getCount();
        right_count = right.getCount();

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            cout << "PUSHBUTTON PRESSED!\n";
            MotorPower(&motors, 50, 0);
        }

        if(right_count >= rot90deg){
            MotorPower(&motors, 0, 0);
        }

    }

}