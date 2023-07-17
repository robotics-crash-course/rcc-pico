#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    int array[100]; //create 1x100 array of integers

    for (int i = 0; i < 100; ++i) { //fill array with values 0-1000
        array[i] = i*10; 
    }

    int i = 0; //new iterator for button to change

    while(true){
        sleep_ms(100);

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            cout << "i = " << i << ", value = " << array[i] << '\n'; //print out index and value of array
            i++; //add one to iterator
        }
    }
}

