#include "rcc_stdlib.h"
using namespace std;

#define WHEEL_RADIUS 0.04 //meters
#define COUNTS_PER_ROTATION 40 
#define TRACK_WIDTH 0.131 //meters

float distanceForArc(float degrees){
    /*
    Calculates the length of an arc want to travel
    Keeping one wheel still, radius is track width
    Portion of circumference is input degrees / 360
    */
    float circumference = 2*3.14*TRACK_WIDTH; //meters
    float ratio = degrees/360.0;
    float distance = circumference*ratio;
    return distance;
}

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

typedef enum{
    WAIT,        //0
    LOOKING,     //1
    FOUND,       //2
    REVERSE,     //3
    TURNING,     //4
    APPROACHING, //5
} state_t;

void print_state(state_t state){
    cout << state << '\n';
}

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 1000);
    MotorsOn(&motors);

    rcc_init_i2c();

    Left_Odom left;
    Right_Odom right;

    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    uint16_t distance;

    state_t state = WAIT;

    int size_spot = 0;
    int tight_turn = distanceToCounts(distanceForArc(45.0));

    while(true) {   
        print_state(state); 

        distance = getFastReading(&lidar); 

        switch(state){
            case WAIT:
                MotorPower(&motors, 0, 0); //stop
                ServoPosition(&s3, 0); //look right
                if(distance > 200){
                    state = LOOKING;
                }
            break;

            case LOOKING:
                MotorPower(&motors, 50, 50); //forwards
                if(distance <= 200){
                    left.setZero(); //reset count
                    state = FOUND;
                }
                break;

            case FOUND:
                sleep_ms(100); 
                if(distance <= 200){
                    size_spot = left.getCount(); 
                    state = REVERSE;
                    left.setZero(); //reset count
                }
                break;

            case REVERSE:
                MotorPower(&motors, -50, -50); //backwards

                //if halfway length of parking spot
                if(left.getCount() > (size_spot/2)){
                    state = TURNING;
                    left.setZero();
                }
                break;

            case TURNING:
                MotorPower(&motors, 50, -50); //rotate right
                if(left.getCount() <= tight_turn){
                    state = APPROACHING;
                }
                break;

            case APPROACHING:
                ServoPosition(&s3, 50); //face forwards
                MotorPower(&motors, 50, 50); //drive forwards
                if(distance <= 100){
                    state = WAIT;
                }
                break;
        }
    }

}