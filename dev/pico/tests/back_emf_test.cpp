#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(28);
    sleep_ms(1000);

    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }  
    
    // cyw43_arch_enable_sta_mode();
    
    cout << "It initalized?" << "\n";

    cyw43_arch_gpio_put(0, true);

    Servo s1;
    Servo s2;
    Servo s3;
    ServoInit(&s1, 16, false, 50);
    ServoInit(&s2, 17, false, 50);
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s1);
    ServoOn(&s2);
    ServoOn(&s3);
    
    adc_select_input(2);
    while(true)
    {   
        uint16_t pot_val = adc_read();
        sleep_ms(10);
        ServoPosition(&s1, 100*pot_val/4097);
        ServoPosition(&s2, 100*pot_val/4097);
        ServoPosition(&s3, 100*pot_val/4097);
        cyw43_arch_gpio_put(0, !(cyw43_arch_gpio_get(0)));
        

    }
}