#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();
    sleep_ms(100);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, true); //led on
    rcc_init_potentiometer();


    CarrieDrive carriedrive;
    CarrieDriveInit(&carriedrive, 16, 17, 18, 19, 1000);
    CarrieDriveOn(&carriedrive);


    while(true)
    {           
        int32_t pot_val = (adc_read()*2) - 4097;
        sleep_ms(10);
        CarrieDrivePower(&carriedrive, 100*pot_val/4097);
        // CarrieDrivePower(&carriedrive, 75);
    }
}