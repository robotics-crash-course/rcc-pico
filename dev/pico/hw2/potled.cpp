#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //led on

    rcc_init_potentiometer(); //setup ADC

    int32_t halfway = 2048;

    while(true){
        int32_t pot_val = adc_read(); //values from 0 to +4097

        if(pot_val > halfway){
            cyw43_arch_gpio_put(0,1); //led on
        }
        else{ //must be less than
            cyw43_arch_gpio_put(0,0); //led off
        }
        sleep_ms(100);
    }
}

