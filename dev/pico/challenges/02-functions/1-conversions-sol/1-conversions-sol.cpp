/*
 * 1-conversions-sol.ino -- unit conversion functions solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include "rcc_stdlib.h"

#define WHEEL_DIAM      0.08
#define CPR 80
#define PI 3.14159
#define DUMMY_TICKS 124

float ticks2meters(unsigned long ticks)
{
  return WHEEL_DIAM*PI/ticks;
}

float ticks2centimeters(unsigned long ticks)
{
  return (WHEEL_DIAM*PI/ticks) * 100.0;
}

int main(void)
{
  stdio_init_all();

  std::cout << "A " << '\n';
  std::cout << DUMMY_TICKS << '\n';
  std::cout << " microsecond duration means an object is " << '\n';
  std::cout << duration2meters(DUMMY_TICKS) << '\n';
  std::cout << " meters or " << '\n';
  std::cout << duration2centimeters(DUMMY_TICKS) << '\n';
  std::cout << " centimeter away\n";
}
