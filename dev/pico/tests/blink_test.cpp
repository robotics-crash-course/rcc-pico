#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();

    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }

    cyw43_arch_gpio_put(0, true);
    
    while(true)
    {   
        sleep_ms(1000);
        cyw43_arch_gpio_put(0, !(cyw43_arch_gpio_get(0))); //blinks W LED

    }

}