#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //led on

    rcc_init_potentiometer(); //setup ADC

    while(true){
        int32_t pot_val = adc_read(); //values from 0 to +4097

        //To scale to -4097 -> +4097
        int32_t scaled_pot_val = (pot_val*2) - 4097;
        //To scale from -100 to 100
        int32_t scaled_again = 100*scaled_pot_val/4097;

        //To scale from 0 -> 100
        int32_t another_scaled_pot_val = 100*pot_val/4097;

        cout << "r chan: " << pot_val << " twice: " << scaled_pot_val << " power: " << scaled_again << " position: " << another_scaled_pot_val<< "\n";
        sleep_ms(100);
    }
}

