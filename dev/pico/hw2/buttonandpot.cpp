#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //turns on LED

    rcc_init_pushbutton(); //set up button
    rcc_init_potentiometer(); //set up pot

    int32_t pot_val;
    int32_t halfway = 2047;
    int32_t tolerance = 10;

    while(true)
    {   
        pot_val = adc_read(); //update pot_val

        if(!gpio_get(RCC_PUSHBUTTON) && ((pot_val - halfway) <= tolerance)) 
        {
            cout << "pumpkin\n"; 
        }
        
        sleep_ms(100);
    }
}


