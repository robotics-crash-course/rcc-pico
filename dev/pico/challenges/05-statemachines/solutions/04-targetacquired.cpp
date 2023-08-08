#include "rcc_stdlib.h"
using namespace std;

typedef enum{
    WAIT,        //0
    SEARCHING,   //1
    FOUND,       //2
    ROTATING_L,  //3
    ROTATING_R,  //4
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

    int positions[100]; //array for servo positions

    for (int i = 0; i < 100; ++i) {
        positions[i] = i;  //filling array 0-99
    }

    int i = 0;
    uint16_t distance;

    state_t state = WAIT;

    while(true) {   
        print_state(state); 

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            state = SEARCHING;
        }

        distance = getFastReading(&lidar); 

        switch(state){
            case WAIT:
                MotorPower(&motors, 0, 0); //stop
                if(distance > 200){
                    state = SEARCHING;
                }
            break;

            case SEARCHING:
                ServoPosition(&s3, positions[i]);
                sleep_ms(100);
                i++;
                if (i >= 100){
                    i = 0; //reset counter
                }
                //transition condition
                if(distance <= 200){
                    state = FOUND;
                }
                break;

            case FOUND:
                ServoPosition(&s3, 50);
                if (i <= 50){  //object was on left
                    state = ROTATING_L;
                }
                else{ //object was on right
                    state = ROTATING_R;
                }
                break;

            case ROTATING_L:
                MotorPower(&motors, -50, 50); //rotate left
                if(distance <= 200){
                    state = APPROACHING;
                }
                break;

            case ROTATING_R:
                MotorPower(&motors, 50, -50); //rotate right
                if(distance <= 200){
                    state = APPROACHING;
                }
                break;

            case APPROACHING:
                MotorPower(&motors, 50, 50); //forwards
                if(distance <= 100){
                    state = WAIT;
                }
                break;

        }


    }

}