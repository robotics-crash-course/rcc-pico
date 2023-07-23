#include "rcc_stdlib.h"
using namespace std;

typedef enum{
    STOP,    //0
    FORWARD, //1
    BACKWARD //2
} state_t;

void print_state(state_t state){
    cout << state << '\n';
}

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

    rcc_init_pushbutton(); //setup button

    state_t state = STOP;

    while(true){

        print_state(state); //testing with function

        //have button start sm
        if(!gpio_get(RCC_PUSHBUTTON)){
            state = FORWARD;
        }   

        distance = getFastReading(&lidar); //check lidar
        
        switch(state){
            case STOP:
                MotorPower(&motors, 0, 0);
                break;

            case FORWARD:
                MotorPower(&motors, 60, 50);
                //transition condition: if object too close
                if(distance < 200){
                    state = BACKWARD;
                }
                break;

            case BACKWARD:
                MotorPower(&motors, -60, -50);
                //transition condition: if object far away
                if(distance >= 200){
                    state = FORWARD;
                }
                break;

            default: 
                state = STOP;

        }

    }

}