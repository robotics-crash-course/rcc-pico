#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();

    rcc_init_potentiometer(); //setup potentiometer on ADC2
    sleep_ms(1000);   

    cyw43_arch_gpio_put(0, true); //led on

    Servo s3; //struct
    ServoInit(&s3, 18, false, 50); //attach to pin 18
    ServoOn(&s3); //enable PWM
    
    while(true)
    {   
        uint16_t pot_val = adc_read(); //get reading from ADC2
        sleep_ms(10);
        
        cout << "Pot val: " << pot_val <<   //raw value
            " pos: " << 180*pot_val/4097 << //scaled value
            "\n";

        ServoPosition(&s3, 100*pot_val/4097); //set to servo position
    }
}