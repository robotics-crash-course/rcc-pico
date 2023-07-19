#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    Left_Odom left; //class
    Right_Odom right; //class
    
    while(true)
    {
        int left_count = left.getCount(); //gets current count
        int right_count = right.getCount();

        if (left_count>=100){
            left.setZero(); //resets count
        }	
        if (right_count>=200){
            right.setZero(); //resets count
        }

        cout << left_count << " | " << right_count << "\n";
        sleep_ms(100);
    }
}
