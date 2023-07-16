#include "rcc_stdlib.h"
#include <bitset>
#include <iomanip>

using namespace std;
uint16_t i=0;
uint8_t ina226_addr_read = 0x41;
uint8_t ina226_addr = 0x40;
const uint8_t vshunt_reg = 0x01;

bool print_things(repeating_timer_t* t)
{
    std::cout << i << '\n';
    return true;
}

int main(void) {
    stdio_init_all();
    rcc_init_i2c();   
    repeating_timer_t print_timer; 
    
    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }

    cyw43_arch_gpio_put(0, true);

    i2c_write_blocking(i2c1, ina226_addr, &vshunt_reg, 1, false);
    add_repeating_timer_ms(1000, print_things, NULL, &print_timer);
    
    while(true){
        uint8_t buffer[2];
        i2c_read_blocking(i2c1, ina226_addr, buffer, 2, false);
        uint16_t data = (uint16_t)buffer[0] << 8 | buffer[1];
        int16_t data2s = ~(data) + 1; //Apply 2s complement to the 16 bits incoming
        // std::cout << std::setprecision(4) << i << " d0: " << 1000.0*(float)data2s*2.5e-6/0.1 << "mA" << '\n';
        i++;

    }
    
}