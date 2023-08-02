#include "rcc_stdlib.h"
using namespace std;

typedef enum{
    //Use commas to separate words
    EXAMPLE, 
    //put rest here...
} state_t;

int main(){
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    //init robot's sensors and actuators here~~

    //rename state_name to something more informative
    state_t state_name = EXAMPLE;

    while(true){
        switch(state_name){
            case EXAMPLE:
                //do something in each state
                //check transition conditions
                break;

            case //PUT REST HERE
        }
    }
}