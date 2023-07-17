#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }
    
    cyw43_arch_gpio_put(0, 1); //turns on led

    cout << "Starting odom test!" << "\n";
    Left_Odom left;
    Right_Odom right;
    
    while(true)
    {
        sleep_ms(100);
        cout << left.getCount() << " | " << right.getCount() << "\n";
			
    }
}
