#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    if(cyw43_arch_init())
    {
        cout << "CYW43 INIT failed!" << '\n';
    }
    cout << "init worked!" << '\n';

    cyw43_arch_gpio_put(0,1);

    //Init pushbutton
    rcc_init_pushbutton();

    //Init servo
    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);
    printf("AFTER SERVO");

    int positions[10];

    for (int i = 0; i < 10; ++i) {
        positions[i] = i*10; 
    }
    int i = 0;

    while(true){
        sleep_ms(100);
        cyw43_arch_gpio_put(0, !(cyw43_arch_gpio_get(0)));

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            cout << "PUSHBUTTON PRESSED!\n";
            ServoPosition(&s3, positions[i]);
            i++;
            if (i >= 9){
                i = 0;
            }
        }
    }
}

