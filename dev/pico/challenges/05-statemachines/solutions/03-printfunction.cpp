#include "rcc_stdlib.h"
using namespace std;

typedef enum{
    STOP,    //0
    FORWARD, //1
    BACKWARD //2
} state_t;

void print_state(state_t state){
    cout << state << '\n';
}

int main(){
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on led

    state_t state = STOP;

    while(true){
        print_state(state); 
    }

}