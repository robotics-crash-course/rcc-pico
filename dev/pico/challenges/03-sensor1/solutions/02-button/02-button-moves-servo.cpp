#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //led on

    rcc_init_pushbutton();

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    int servo_pos = 0;

    while(true){
        sleep_ms(100);

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            servo_pos+=10; //adds 10 to position

            if (servo_pos > 100){ //if position over 100, send back to 0
                servo_pos = 0;
            }
        }

        ServoPosition(&s3, servo_pos); //moves servo
    }
}

