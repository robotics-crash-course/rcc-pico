// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 2-fizz-buzz-sol.cpp -- FizzBuzz solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Michael Giglia <michael.a.giglia@gmail.com>
 */
#include "rcc_stdlib.h"
#define BLINK_sleep_ms_MS 200
#define CNT_sleep_ms_MS   400


void blink_cnt(int cnt)
{
  for (int i = 0; i < cnt; i++) {
    cyw43_arch_gpio_put(0, true);
    sleep_ms(BLINK_sleep_ms_MS);
    cyw43_arch_gpio_put(0, false);
    sleep_ms(BLINK_sleep_ms_MS);
  }
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


  // ensure 0 is off before we begin
  cyw43_arch_gpio_put(0, false);

  for (int i = 1; i <= 100; i++) {
    if (!(i % 15)) {
      std::cout << "FizzBuzz" << '\n';
      blink_cnt(3);
    } else if (!(i % 5)) {
      std::cout << "Buzz" << '\n';
      blink_cnt(2);
    } else if (!(i % 3)) {
      std::cout << "Fizz" << '\n';
      blink_cnt(1);
    } else {
      std::cout << i << '\n';
    }

    sleep_ms(CNT_sleep_ms_MS);
  }
}