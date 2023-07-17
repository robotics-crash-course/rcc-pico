#include "rcc_stdlib.h"
using namespace std;

int main()
{   
    stdio_init_all();
    sleep_ms(100);
    cyw43_arch_init(); //setup 
    cyw43_arch_gpio_put(0, 1); //turns on led

    while(true)
    {
        cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //blinks LED
        sleep_ms(300);
        cout <<"yo"<<'\n';

    }
}