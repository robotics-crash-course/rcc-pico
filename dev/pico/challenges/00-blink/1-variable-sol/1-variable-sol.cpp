// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 1-variable-sol.cpp -- variable length blinking solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Andy Jaku <andyjaku13@gmail.com>
 *
 */

#include "rcc_stdlib.h"
using namespace std;

int main()
{
  stdio_init_all();

  if(cyw43_arch_init())
  {
    cout << "CYW43 INIT failed!" << '\n';
  }
  cout << "init worked!" << '\n';

  for (int i = 10; i <= 1000; i += 10) {
    cyw43_arch_gpio_put(0, false);
    sleep_ms(i);
    cyw43_arch_gpio_put(0, true);
    sleep_ms(i);
  }
}