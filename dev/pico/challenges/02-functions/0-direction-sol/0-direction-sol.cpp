// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 0-direction-sol.cpp -- direction functions solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Michael Giglia <michael.a.giglia@gmail.com>
 */

#include "rcc_stdlib.h"


#define MOVEMENT_sleep_ms_MS 2000

/* Declare Motor struct.
We declare in global variable space so that our motor control functions
do not require us to pass in the pointer to this struct.
We can of course have the motor control functions take as an input argument
the pointer to the Motor struct
*/
Motor mot;

void forward(int pwr)
{
  MotorPower(&mot, pwr, pwr);
}

void backward(int pwr)
{
  MotorPower(&mot, -pwr, -pwr);
}

void left(int pwr)
{
  MotorPower(&mot, 0, pwr);
}

void right(int pwr)
{
  MotorPower(&mot, pwr, 0);
}

int main(void)
{
  stdio_init_all();
  sleep_ms(1500);

  //Initialize the cyw43 chip (needed to use onboard LED!)
  if(cyw43_arch_init())
  {
      std::cout << "CYW43 INIT failed!" << '\n';
      return 1;
  }
  std::cout << "CYW43 initialized!" << '\n';

  // initialize motor hardware
  MotorInit(&mot, RCC_ENA, RCC_ENB, 20000);
  MotorsOn(&mot);

  std::cout << "going forward!" << '\n';
  forward(100);
  sleep_ms(MOVEMENT_sleep_ms_MS);

  std::cout << "going backward!" << '\n';
  backward(100);
  sleep_ms(MOVEMENT_sleep_ms_MS);

  std::cout << "going left!" << '\n';
  left(100);
  sleep_ms(MOVEMENT_sleep_ms_MS);

  std::cout << "going right!" << '\n';
  right(100);
  sleep_ms(MOVEMENT_sleep_ms_MS);

  MotorsOff(&mot);
}
