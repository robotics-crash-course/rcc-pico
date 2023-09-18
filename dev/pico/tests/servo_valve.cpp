#include "rcc_stdlib.h"
using namespace std;

PID_control_config_t con;

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
    
    //Init servo valve pot
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_select_input(0);
    int servo_pot;
    int plc_val;
    float desired = 2000;

    while(true) {
        // Get reading from Potentiometer
        adc_select_input(0); 
        servo_pot = adc_read();
        // Get reading from PLC
        adc_select_input(1);
        plc_val = adc_read();

        //Do controls
        float output = controller.pid(plc_val, servo_pot);

        //Send controller output
        MotorPower(&motors, output, 0);

        //print
        cout << "servo_pot: " << servo_pot << " | output: " << output << "\n";
    }
    
}