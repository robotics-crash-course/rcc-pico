#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    int positions[10]; //create 1x10 array of integers

    for (int i = 0; i < 10; ++i) { //fill array with values 0-100
        positions[i] = i*10; 
    }

    int i = 0; //iterator for servo positions

    while(true){
        sleep_ms(100);

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            ServoPosition(&s3, positions[i]); //move servo

            i++; //add 1 to i 

            if (i > 9){ //if gotten to end of array, reset
                i = 0;
            }
        }
    }
}

