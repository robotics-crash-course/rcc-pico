#include "rcc_stdlib.h"
#include "pico/mutex.h"
using namespace std;

uint16_t led_count=0;

typedef enum {
  RESET,
  RESET_REQUEST,
  TRIGGER_REQUEST,
  TRIGGERED,
  ERROR
}TARGET_STATE;

typedef struct state_s{
uint32_t time_in_trigger;
uint32_t time_in_button;
TARGET_STATE current_state;
TARGET_STATE requested_state;
mutex_t mtx;
}state_t;

state_t state;

const uint32_t button_events = GPIO_IRQ_EDGE_FALL;
const uint32_t trigger_events = GPIO_IRQ_EDGE_RISE;

void button_callback(void)
{
    if(gpio_get_irq_event_mask(RCC_PUSHBUTTON) & button_events){
        gpio_acknowledge_irq(RCC_PUSHBUTTON, button_events);
        cout << "RESET CALLBACK\n";
        state.requested_state = RESET_REQUEST;
        // gpio_set_irq_enabled(RCC_PUSHBUTTON, button_events, false);
        }
}

void trigger_callback(void)
{
    if(gpio_get_irq_event_mask(18) & trigger_events) 
    { 
        gpio_acknowledge_irq(18, trigger_events);
        cout << "TRIGGER CALLBACK\n";
        state.requested_state = TRIGGER_REQUEST;
        // gpio_set_irq_enabled(18, trigger_events, true);
    }
}

bool state_machine_timer_callback(struct repeating_timer *t)
{
    // state_t* s = (state_t*)t->user_data;
    // uint32_t owner;
    // if(!mutex_try_enter(&s->mtx, &owner)){
        switch(state.current_state)
        {
            case RESET:
                if(led_count%10 == 0){
                    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0));
                }
                led_count++;
                if(state.requested_state == TRIGGER_REQUEST)
                {
                    state.current_state = TRIGGER_REQUEST;
                    state.time_in_trigger = time_us_32();
                    cout << "Transisition to TRIGGERED_REQUEST\n";
                }    
                break;
            case TRIGGER_REQUEST:
                if(time_us_32() - state.time_in_trigger >= 10000)
                {
                    state.current_state = TRIGGERED;
                    cout << "Transisition to TRIGGERED\n";
                    gpio_set_irq_enabled(18, trigger_events, true);
                }
                break;
            case TRIGGERED:
                cyw43_arch_gpio_put(0, 1);
                if(state.requested_state == RESET_REQUEST){
                    state.current_state = RESET_REQUEST;
                    state.time_in_button = time_us_32();
                    cout << "Transisition to RESET_REQUEST\n";
                }
                break;
            case RESET_REQUEST:
                if(time_us_32() - state.time_in_button >= 10000)
                {
                    state.current_state = RESET;
                    gpio_set_irq_enabled(RCC_PUSHBUTTON, button_events, true);
                }
                break;
            case ERROR:
            break;
        }
    // }
    return true;
}

int main()
{
    stdio_init_all();
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1); //turns on LED

    rcc_init_pushbutton(); //set up button
    // gpio_init(18);
    // gpio_pull_up(18);
    // gpio_set_dir(18, false);
    mutex_init(&state.mtx);

    gpio_set_input_enabled(18, true);
    gpio_set_input_enabled(RCC_PUSHBUTTON, true);
    gpio_set_irq_enabled(RCC_PUSHBUTTON, button_events, true);
    gpio_set_irq_enabled(18, trigger_events, true);
    gpio_add_raw_irq_handler(RCC_PUSHBUTTON, button_callback);
    gpio_add_raw_irq_handler(18, trigger_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);

    repeating_timer_t sm_timer;
    add_repeating_timer_ms(10, state_machine_timer_callback, &state, &sm_timer);
    while(1);
}
