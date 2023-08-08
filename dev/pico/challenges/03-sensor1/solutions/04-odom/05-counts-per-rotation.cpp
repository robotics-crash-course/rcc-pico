#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    Left_Odom left; //struct
    Right_Odom right; //struct
    
    while(true)
    {
        int left_count = left.getCount(); 
        int right_count = right.getCount();

        //rotate wheel with your hands or fingers
        //should see 40 counts per rotation

        //10 rotations is 400 counts!

        cout << left_count << " | " << right_count << "\n";
        sleep_ms(100);
    }
}
