#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //turns on LED

    rcc_init_pushbutton(); //set up button

    bool toggle = false;

    while(true)
    {   
        if(!gpio_get(RCC_PUSHBUTTON)) //if NOT gpio (if gpio is low)
        {
            toggle = !toggle; //toggle toggle lol
            
            if(toggle){
                cout << "yo\n"; 
            }
            else{
                cout << "hey\n"; 
            }
        }
    }
}



