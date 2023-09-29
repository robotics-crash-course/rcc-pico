#include "rcc_stdlib.h"
using namespace std;

#define HALFWAY 2000

struct raft_sensors{
    int32_t pot_val;
    bool button_state;
};

struct raft_sensors myraft;

void change_led(struct raft_sensors* r){
    if(!(r->button_state)){ //if button not pressed
        return; //dont change state
    }
    else{
        if(r->pot_val >= HALFWAY){
            cyw43_arch_gpio_put(0,1); //led on
        }
        else{
            cyw43_arch_gpio_put(0,0); //led off
        }

    }
}

void update_sensor_data(struct raft_sensors* r){
    r->pot_val = adc_read();
    r->button_state = !gpio_get(RCC_PUSHBUTTON); //true = pressed
    change_led(r);
}

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //turns on LED

    rcc_init_pushbutton(); //set up button
    rcc_init_potentiometer(); //set up pot

    while(true)
    {   
        update_sensor_data(&myraft);
        sleep_ms(100);
    }
}


