#include "rcc_stdlib.h"
using namespace std;

//function returns void and inputs integer
void blink_led(int time){
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //blinks LED
    sleep_ms(time);
}

int main()
{   
    stdio_init_all();
    sleep_ms(100);
    cyw43_arch_init(); //setup 
    cyw43_arch_gpio_put(0, 1); //turns on led

    rcc_init_pushbutton(); //init button

    bool blinking = false;

    while(true)
    {
        if(!gpio_get(RCC_PUSHBUTTON)){
            blinking = true;
        }

        if (blinking){
            blink_led(300);
        }
    }
}