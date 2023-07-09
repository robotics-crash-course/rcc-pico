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
    Servo s3;
    ServoInit(&s1, 16, false, 50);
    ServoInit(&s2, 17, false, 50);
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s1);
    ServoOn(&s2);
    ServoOn(&s3);
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

    int positions[10];

    for (int i = 0; i < 10; ++i) {
        positions[i] = i*10; 
    }

    int i = 0;
    uint16_t distance;

    //this would be so much easier with a state machine tbh
    while(true) {   

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            cout << "PUSHBUTTON PRESSED!\n";

            while(true){
                distance = getFastReading(&lidar);

                if(distance >= 200){
                    ServoPosition(&s3, positions[i]);
                    i++;
                    if (i >= 9){
                        i = 0;
                    }
                }
                
                if(distance < 200){
                    ServoPosition(&s3, 50); //face forward
                    if (i <= 5){  //object was on left
                        MotorPower(&motors, 50, -50); //turn left
                        break; 
                    }
                    if (i > 5){ //object was on right
                        MotorPower(&motors, -50, 50); //turn right
                        break;
                    }
                }
            }

            while(true){
                distance = getFastReading(&lidar);
                if(distance < 200){
                    MotorPower(&motors, 0, 0); //stops when found object
                    //flashier would be to go forward at this point until distance < 20
                }
            }

        }

    }

}