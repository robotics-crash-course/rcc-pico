#include "rcc_stdlib.h"
using namespace std;

#define GPIO_IN 3

int main()
{   
    stdio_init_all();
    sleep_ms(100);
    cyw43_arch_init(); //setup 
    cyw43_arch_gpio_put(0, 1); //turns on led

    gpio_init(GPIO_IN); 
    gpio_set_dir(GPIO_IN, false); //input

    while(true)
    {
        
        if(gpio_get(GPIO_IN)){
            cout << "gpio on\n";
        } 
        else if(!gpio_get(GPIO_IN)){
            cout << "gpio off\n";
        }
        else{
            cout << "floating\n"; //just incase
        }
    }
}