#include "rcc_stdlib.h"
using namespace std;

struct sensor_data{
    int pot_val;
    int btn_val;
};

void update_sensor_data(struct sensor_data* d)
{
    //Update sensor_data
    d->btn_val=gpio_get(RCC_PUSHBUTTON);
    d->pot_val=adc_read();
    update_led(d);
}

void update_led(struct sensor_data* d)
{
    if(d->btn_val == 1) { return; }
    if(d->btn_val == 0)
    {
        if(d->pot_val > 2000) { cyw43_arch_gpio_put(0, 1); }
        if(d->pot_val <= 2000) { cyw43_arch_gpio_put(0, 0); }
        
    }
}

sensor_data s_data;

int main(void) {
    stdio_init_all();
    sleep_ms(1000);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_potentiometer();
    rcc_init_pushbutton();

    while(true)
    {
        update_sensor_data(&s_data);
        sleep_ms(100);
    }
}