#include "rcc_stdlib.h"
using namespace std;

int main()
{   
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init(); //setup 
    cyw43_arch_gpio_put(0, 1); //turns on led

    while(true)
    {
        //each for loop runs 3 times
        for(int i = 0; i < 3; i++){
            cyw43_arch_gpio_put(0, true); //on 
            sleep_ms(100); 
            cyw43_arch_gpio_put(0, false); //off
            sleep_ms(100); 
        }
        for(int i = 0; i < 3; i++){
            cyw43_arch_gpio_put(0, true); //on 
            sleep_ms(300); 
            cyw43_arch_gpio_put(0, false); //off
            sleep_ms(300); 
        }
        for(int i = 0; i < 3; i++){
            cyw43_arch_gpio_put(0, true); //on 
            sleep_ms(100); 
            cyw43_arch_gpio_put(0, false); //off
            sleep_ms(100); 
        }
    }
}