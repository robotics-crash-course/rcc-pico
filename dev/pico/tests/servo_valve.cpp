#include "rcc_stdlib.h"
using namespace std;
#define MOV_AVG_SIZE 10
#define ADC_REF 3.3
#define SIG_MAX 3.1
#define SIG_MIN 0.7
#define SIG_SAFE 3.0
#define ADC_OUT_MAX 4097
#define THRESH_HIGH (ADC_OUT_MAX*(SIG_MIN+0.2)/ADC_REF)
#define THRESH_LOW (ADC_OUT_MAX*(SIG_MIN-0.2)/ADC_REF)

PID_control_config_t con;

typedef struct avg_s{
    int avg[MOV_AVG_SIZE];
    int index=0;
}avg_t;

enum hysteresis_state{
    LOW=0,
    HIGH=1
};

typedef struct hysteresis_s{
    int state=hysteresis_state::LOW;
    int thresh=THRESH_HIGH;
} hysteresis_t;

void update_hysteresis(hysteresis_t* h, int raw)
{
    if(raw <= THRESH_LOW)
    {
        h->state = hysteresis_state::LOW;
        h->thresh = THRESH_HIGH;

    }
    if(raw >= THRESH_HIGH)
    {
        h->state = hysteresis_state::HIGH;
        h->thresh = THRESH_LOW;
    }
}

int sample(avg_t* a, int sample)
{
    a->index++;
    if(a->index>=MOV_AVG_SIZE){ a->index=0; }
    a->avg[a->index]=sample;
    float calc=0;
    for(int i=0; i<MOV_AVG_SIZE; i++)
    {
        calc+=a->avg[i];
    }
    return static_cast<int>(calc/MOV_AVG_SIZE);
}

int main(void){
    stdio_init_all();
    sleep_ms(1000);

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);    
    
    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }
    cyw43_arch_gpio_put(0, true);

    con.kp = 1;
    con.ki = 0.02;
    con.kd = 0.0;
    con.ts = 0.01; // 10ms
    con.sigma = 0.1;
    con.lowerLimit = -100;
    con.upperLimit = 100;
    con.antiWindupEnabled = true;
    
    PID_control controller(con);
    controller.setDeadbands(0, 30);
    Differentiator diff(con.sigma, con.ts);
    avg_t mov_avg;

    //Init servo valve pot
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_select_input(0);
    int servo_pot;
    int plc_val;
    float desired = 2000;
    int plc_raw;

    hysteresis_t h_state;

    while(true) {
        // Get reading from Potentiometer
        adc_select_input(0); 
        servo_pot = adc_read();
        // Get reading from PLC
        adc_select_input(1);
        plc_val = adc_read();
        plc_raw = plc_val;
        float plc_float = static_cast<float>(plc_val);

// Check safe values from adc
        if(plc_float >= (ADC_OUT_MAX*SIG_MAX/ADC_REF) || plc_float <= (ADC_OUT_MAX*SIG_MIN/ADC_REF))
        {
            //The voltage input is above 3.1V or < 0.5V, so force plc_val to be full open(3v)
            plc_val = static_cast<int>(SIG_SAFE*ADC_OUT_MAX/ADC_REF);
            cout << "\n" << "ERROR, BAD ADC SIG\n";
        }


//Calc moving average (after input is agumented if not safe`)
        int average = sample(&mov_avg, plc_val);
        // update_hysteresis(&h_state, plc_val);
        //Do controls
        float output = controller.pid(average, servo_pot);

        //Send controller output
        MotorPower(&motors, output, 0);

        //print for debug
        // cout << "raw: " << plc_raw << " | h_state: " << h_state.state << '\n';
        cout << "raw: " << plc_raw << " | plc_val: " << plc_val << " | avg: " << average << " | servo_pot: " << servo_pot << " | output: " << output << "\n";
    }
    
}