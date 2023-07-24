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
    FORWARD, 
    TURN, 
    STOP
} state_t;

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

    int left_count = 0;
    int right_count = 0;

    int meter_counts = distanceToCounts(1.0);
    int arc90_counts = distanceToCounts(distanceForArc(90.0));

    int times_turned = 0;
    int times_need_to_turn = 20; //5 times around square

    state_t state = FORWARD; 


    while(true)
    {
        left_count = left.getCount();
        right_count = right.getCount();

        switch(state){
            case FORWARD:
                MotorPower(&motors, 50, 50);
                if (left_count > meter_counts){
                    left.setZero(); //reset count
                    state = TURN;
                }
                if(times_turned > times_need_to_turn){
                    state = STOP;
                }
                break;

            case TURN:
                MotorPower(&motors, 50, 0); //right turn
                if (left_count >= arc90_counts){
                    times_turned++;
                    left.setZero(); //reset count
                    state = FORWARD;
                }
                break;

            case STOP:
                MotorPower(&motors, 0, 0); //stop
                break;
        }
    }

}

