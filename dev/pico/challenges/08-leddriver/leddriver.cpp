#include "rcc_stdlib.h"
using namespace std;

//led gpios
#define BLUE 0
#define RED 1
#define GREEN 2

typedef enum{
    OFF, //0
    ON   //1
} state_t;


void rcc_init_leddriver(void)
{
    gpio_init(BLUE); 
    gpio_set_dir(BLUE, true); //output
    gpio_init(RED); 
    gpio_set_dir(RED, true); //output
    gpio_init(GREEN); 
    gpio_set_dir(GREEN, true); //output
}

int main(){
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0, 1); //turns on pico led

    rcc_init_leddriver();

    //setup timing variables
    int64_t blue_current_time = time_us_64(); 
    int64_t blue_previous_time = time_us_64(); 
    int64_t red_current_time = time_us_64(); 
    int64_t red_previous_time = time_us_64(); 
    int64_t green_current_time = time_us_64(); 
    int64_t green_previous_time = time_us_64(); 

    //400 milliseconds converted to microseconds
    int blink_time = 400000; 

    state_t blue_state = OFF; //initial is OFF
    state_t red_state = OFF; //initial is OFF
    state_t green_state = OFF; //initial is OFF

    while(true){
        //update current time
        blue_current_time = time_us_64(); 
        green_current_time = time_us_64();
        red_current_time = time_us_64();



        switch(blue_state){
            case ON:
                gpio_put(BLUE, true);

                //transition condition: has blink_time elapsed
                if(blue_current_time - blue_previous_time >= blink_time){
                    blue_state = OFF;
                    blue_previous_time = blue_current_time; 
                }
                break;

            case OFF:
                gpio_put(BLUE, false); //led off

                //transition condition: has blink_time elapsed
                if(blue_current_time - blue_previous_time >= blink_time){
                    blue_state = ON;
                    blue_previous_time = blue_current_time; 
                }
                break;
        }

        switch(green_state){
            case ON:
                gpio_put(GREEN, true);

                //transition condition: has blink_time elapsed
                if(green_current_time - green_previous_time >= 2*blink_time){
                    green_state = OFF;
                    green_previous_time = green_current_time; 
                }
                break;

            case OFF:
                gpio_put(GREEN, false); //led off

                //transition condition: has blink_time elapsed
                if(green_current_time - green_previous_time >= 2*blink_time){
                    green_state = ON;
                    green_previous_time = green_current_time; 
                }
                break;
        }

        switch(red_state){
            case ON:
                gpio_put(RED, true);

                //transition condition: has blink_time elapsed
                if(red_current_time - red_previous_time >= 3*blink_time){
                    red_state = OFF;
                    red_previous_time = red_current_time; 
                }
                break;

            case OFF:
                gpio_put(RED, false); //led off

                //transition condition: has blink_time elapsed
                if(red_current_time - red_previous_time >= 3*blink_time){
                    red_state = ON;
                    red_previous_time = red_current_time; 
                }
                break;
        }
    }
}