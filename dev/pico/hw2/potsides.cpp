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
            cout << "to the right to the right\n";
        }
        else if(pot_val < halfway){
            cout << "to the left to the left\n";
        }
        else if(pot_val = halfway){
            cout << "halfway\n";
        }
        else{
            cout << "out of bounds?!?!\n";
        }

        sleep_ms(100);
    }
}

