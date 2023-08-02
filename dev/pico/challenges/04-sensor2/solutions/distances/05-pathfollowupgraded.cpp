#include "rcc_stdlib.h"
using namespace std;

//global
#define rot90     65
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

void blink_led(int time){
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //blinks LED
    sleep_ms(time);
}

void move_forward(Motor* motorPointer, Left_Odom* left, Right_Odom* right, int count) {
    int initial_left = left->getCount();
    int initial_right = right->getCount();

    int current_left, current_right;

    while ((current_left < initial_left + count) && (current_right < initial_right + count)) {
        MotorPower(motorPointer, 50, 50);
        current_left = left->getCount();
        current_right = right->getCount();
    }
}

void turn(Motor* motorPointer, Left_Odom* left, Right_Odom* right, int counts, bool dir) {
    // direction bool: False = Left, True = Right

    int initial_count;
    int current_count;

    if (dir) {
        initial_count = right->getCount();
    } else {
        initial_count = left->getCount();
    }

    while (current_count < (initial_count + counts)) {
        if (dir) {
            MotorPower(motorPointer, 0, 50);
            current_count = right->getCount();
        } else {
            MotorPower(motorPointer, 50, 0);
            current_count = left->getCount();
        }
        sleep_ms(100);
    }
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

    int left_count = 0;
    int right_count = 0;

    bool blinking = false;

    int halfmeter_counts = distanceToCounts(0.5);
    int meter_counts = distanceToCounts(1.0);
    int arc90_counts = distanceToCounts(distanceForArc(90.0));

    while(true)
    {
        left_count = left.getCount();
        right_count = right.getCount();

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            //forward 0.5 meters
            move_forward(&motors, &left, &right, halfmeter_counts); 
            sleep_ms(200);
            //turn right 90 degrees
            //this method calculates distance along arc and converts to counts
            turn(&motors, &left, &right, arc90_counts, false); 
            sleep_ms(200);
            //move forward 1.0 meters
            move_forward(&motors, &left, &right, meter_counts); 
            //stop & blink!
            MotorPower(&motors, 0, 0); 
            blinking = true;
        }

        if(blinking){
            blink_led(300); //blinks!
        }
    }

}