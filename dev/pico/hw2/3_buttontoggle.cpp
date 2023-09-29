#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //turns on LED

    rcc_init_pushbutton(); //set up button

    while(true)
    {   
        if(!gpio_get(RCC_PUSHBUTTON)) //if button pressed
        {
            cout << "yo\n"; 
        }
        else{ //button not pressed
            cout << "hey\n"; 
        }
    }
}



