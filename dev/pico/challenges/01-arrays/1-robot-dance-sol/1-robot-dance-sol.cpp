/*
 * 1-robot-dance-sol.cpp -- robot dance solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2023  Andy Jaku <andyjaku13@gmail.com>
 *
 */

#include "rcc_stdlib.h"

int main(void)
{
    stdio_init_all();

    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 1000);
    MotorsOn(&motors);

    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }

    cyw43_arch_gpio_put(0, true);

    // dance[0] ==> left motor pwm
    // dance[1] ==> right motor pwm
    // dance[2] ==> millisecond delays
    int dance[3][12] = {
      {100,    0,  200, -100,    0,   0,   50,  200, -200, -150,  200, -50},
      {100,  200,    0, -100, -200,   0,  -25,  200, -200,  150, -200, -50},
      {800, 1000, 1000,  800, 2000, 500, 4000, 2000, 2000, 1000, 4000, 800},
    };

    for (int i = 0; i < 6; i++) {
      MotorPower(&motors, dance[0][11 - i], dance[1][11 - i]);
      sleep_ms(dance[2][11 - i]);
      MotorPower(&motors, dance[0][i], dance[1][i]);
      sleep_ms(dance[2][i]);
    }
}
