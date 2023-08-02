#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1500);

    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();
    rcc_init_potentiometer();

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);

    float scale_factor = 1.2;
    bool test_state = false;
    //using this state so that the pot can live update the base_power

    while(true)
    {
        uint16_t pot_val = adc_read();
        int base_power = static_cast<int>((100*pot_val)/4097); // Scale power from 0-100%

        if (!gpio_get(RCC_PUSHBUTTON))
        {
            test_state = !test_state;
        }

        if (test_state)
        {
            MotorPower(&motors, static_cast<int>(base_power*scale_factor), base_power);
            sleep_ms(100);
        }
    }

}