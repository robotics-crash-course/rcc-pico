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
    sleep_ms(1000); //wait a second

    while(true)
    {
        blink_led(300); //inputs 0.3 seconds
    }
}