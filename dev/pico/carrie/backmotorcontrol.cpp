#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();
    sleep_ms(100);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, true); //led on

    CarrieDrive carriedrive;
    CarrieDriveInit(&carriedrive, 16, 17, 18, 19, 1000);
    CarrieDriveOn(&carriedrive);


    while(true)
    {   
        CarrieDrivePower(&carriedrive, 75);
    }
}