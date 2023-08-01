#include "rcc_stdlib.h"
using namespace std;

//led gpios
#define BLUE 0
#define RED 1
#define GREEN 2

typedef enum{
    REDTOBLUE, //0
    BLUETOGREEN,   //1
    GREENTORED //2
} state_t;


int main(){
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on pico led

    RGBled led;

    LedInit(&led, RED, GREEN, BLUE, 500);
    LEDOn(&led);

    state_t led_state = REDTOBLUE; 

    //setup timing variables
    int64_t current_time = time_us_64(); 
    int64_t previous_time = time_us_64(); 
    int64_t fade_time = 100000; 

    int fade_power_down = 100;
    int fade_power_up = 0;

    while(true){
        //update current time
        current_time = time_us_64();

        switch(led_state){
            case REDTOBLUE:

                if(current_time - previous_time >= fade_time){
                    fade_power_down -= 10; 
                    fade_power_up += 10;
                    LEDPower(&led, fade_power_down, 0, fade_power_up);

                    if(fade_power_down <= 10){
                        led_state = BLUETOGREEN;
                        fade_power_down = 100;
                        fade_power_up = 0;
                    }
                    previous_time = current_time; 
                }
                break;
            case BLUETOGREEN:

                if(current_time - previous_time >= fade_time){
                    fade_power_down -= 10; 
                    fade_power_up += 10;
                    LEDPower(&led, 0, fade_power_up, fade_power_down);

                    if(fade_power_down <= 10){
                        led_state = GREENTORED;
                        fade_power_down = 100;
                        fade_power_up = 0;
                    }
                    previous_time = current_time; 
                }
                break;
            case GREENTORED:

                if(current_time - previous_time >= fade_time){
                    fade_power_down -= 10; 
                    fade_power_up += 10;
                    LEDPower(&led, fade_power_up, fade_power_down, 0);

                    if(fade_power_down <= 10){
                        led_state = REDTOBLUE;
                        fade_power_down = 100;
                        fade_power_up = 0;
                    }
                    previous_time = current_time; 
                }

                break;

        }

    }
}