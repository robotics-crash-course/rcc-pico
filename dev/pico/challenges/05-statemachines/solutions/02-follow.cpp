#include "rcc_stdlib.h"
using namespace std;

typedef enum{
    WAIT,    //0
    FORWARD, //1
    BACKWARD //2
} state_t;

int main(){
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    Motor motors; 
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000); 
    MotorsOn(&motors);

    rcc_init_i2c();
    VL53L0X lidar;
    rcc_init_lidar(&lidar);
    uint16_t distance;

    int goal_dist = 200; //mm
    int tolerance = 50; //mm

    state_t state = WAIT; //initial state

    while(true){

        distance = getFastReading(&lidar); //check lidar
        cout << "distance: " << distance << " || state: " << state << '\n';

        switch(state){

            case WAIT:
                MotorPower(&motors, 0, 0); //stop
                //transition conditions:
                if(distance >= (goal_dist + tolerance)){ 
                    state = FORWARD;
                }
                if(distance <= (goal_dist - tolerance)){ 
                    state = BACKWARD;
                }
                break;

            case FORWARD:
                MotorPower(&motors, 60, 60); //forwards
                //transition conditions: 
                if(distance <= (goal_dist + tolerance)){ 
                    state = WAIT;
                }
                break;

            case BACKWARD:
                MotorPower(&motors, -60, -60); //backwards
                //transition conditions:
                if(distance >= (goal_dist - tolerance)){ 
                    state = WAIT;
                }
                break;
        }

    }

}