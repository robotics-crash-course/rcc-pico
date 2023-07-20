#pragma once


//arm-none-eabi stdlib.h?
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>


//Pico SDK
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h" //Imported to blink LED
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

//Rcc made
#include "rcc_pins.h"
#include "pwm_helper.h"
#include "sensors/mpu6050.h"
#include "sensors/VL53L0X.h"
#include "sensors/hc-sr04.h"
#include "sensors/odom.h"
#include "util/pid-control.h"
#include "util/differentiator.h"
#include "util/integral.h"


#define ARRAYSIZE(X) (sizeof(X) / sizeof((X)[0]))

// Potentiometer Init
void rcc_init_potentiometer(void)
{

    adc_init();
    adc_gpio_init(RCC_POTENTIOMETER);
    adc_select_input(2);
}
//Push button init
void rcc_init_pushbutton(void)
{
    gpio_init(RCC_PUSHBUTTON);
    gpio_pull_up(RCC_PUSHBUTTON);
    gpio_set_dir(RCC_PUSHBUTTON, false);
}
//I2C init
void rcc_init_i2c(void)
{
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(RCC_I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_function(RCC_I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(RCC_I2C_SCL);
    gpio_pull_up(RCC_I2C_SDA);
}