#include "rcc_stdlib.h"
using namespace std;

//define global constants 
#define WHEEL_RADIUS 0.04 //meters
#define COUNTS_PER_ROTATION 40 

int distanceToCounts(float distance){
    /*
    converts distance in meters [m] to encoder counts 
    takes in distance as a float
    returns integer
    */
    float rotations = distance/(2*3.14*WHEEL_RADIUS);
    float counts_f = rotations*COUNTS_PER_ROTATION;
    int counts = (int)counts_f;
    return counts;
}

float countsToDistance(int counts){
    /*
    converts encoder counts to distance in meters [m]
    takes in counts as integer
    returns float
    */
    float rotations = (float)counts/COUNTS_PER_ROTATION;
    float distance = rotations*((2*3.14*WHEEL_RADIUS));
    return distance;
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
    printf("AFTER MOTORS");

    rcc_init_i2c();
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    Left_Odom left;
    Right_Odom right;

    int left_count = 0;
    int right_count = 0;

    uint16_t distance;

    float left_traveled;
    float right_traveled;

    bool searching = false;
    bool object_found = false;

    while(true){   

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            ServoPosition(&s3, 50); //look forwards
            searching = true;
        }

        //in while loop update!
        distance = getFastReading(&lidar); //check lidar

        //update counts while moving
        left_count = left.getCount(); 
        right_count = right.getCount();

        if(searching){

            MotorPower(&motors, 60, 50); //go forwards

            if(distance <= 200){
                //estimate distance traveled
                left_traveled = countsToDistance(left_count);
                right_traveled = countsToDistance(right_count);
                //reset counts
                left.setZero();
                right.setZero();
                searching = false;
                object_found = true;
            }
        }

        if(object_found){
            MotorPower(&motors, -60, -50); //go backwards

            //convert distance traveled back to counts
            if(left_count >= distanceToCounts(left_traveled)){
                MotorPower(&motors, 0, 0); //stop
            }
        }
    }

}