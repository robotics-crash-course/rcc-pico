#include "rcc_stdlib.h"
using namespace std;

int main()
{   
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init(); //setup 
    cyw43_arch_gpio_put(0, 1); //turns on led
    sleep_ms(100); //wait a bit

    while(true)
    {
        cyw43_arch_gpio_put(0, 0); //turns off to start

        for(int i; i < 3; i++){
            cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //on off on
            sleep_ms(300);
        }
        for(int i; i < 3; i++){
            cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //on off on
            sleep_ms(100);
        }
        for(int i; i < 3; i++){
            cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //on off on
            sleep_ms(300);
        }
    }
}