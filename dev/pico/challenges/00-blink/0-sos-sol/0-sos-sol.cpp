// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 0-sos-sol.cpp -- SOS solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Michael Giglia <michael.a.giglia@gmail.com>
 */

#include "rcc_stdlib.h"
#define SHORT_MS 400
#define LONG_MS  800
#define PAUSE_MS 600

int main()
{
  //Initialize stdio so that usb/serial comms work!
  stdio_init_all();    
  
  //Initialize the cyw43 chip (needed to use onboard LED!)
  if(cyw43_arch_init())
    {
        cout << "CYW43 INIT failed!" << '\n';
        return 1;
    }
    cout << "CYW43 initialized!" << '\n';


  // S
  cyw43_arch_gpio_put(0, true);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, true);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, true);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(SHORT_MS);

  // O
  cyw43_arch_gpio_put(0, true);
  sleep_ms(LONG_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(LONG_MS);
  cyw43_arch_gpio_put(0, true);
  sleep_ms(LONG_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(LONG_MS);
  cyw43_arch_gpio_put(0, true);
  sleep_ms(LONG_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(LONG_MS);

  // S
  cyw43_arch_gpio_put(0, true);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, true);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, true);
  sleep_ms(SHORT_MS);
  cyw43_arch_gpio_put(0, false);
  sleep_ms(SHORT_MS);

  // (optional) visible pause
  sleep_ms(PAUSE_MS);
}
