// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * 0-random-longs-sol.cpp -- generate random longs solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Michael Giglia <michael.a.giglia@gmail.com>
 */
#include "rcc_stdlib.h"

int main(void)
{
  long randoms[16];
  //Initialize stdio so that usb/serial comms work!
  stdio_init_all();  

  //Pause so our computer has time to connect via serial! 
  sleep_ms(1500); 
  
  //Initialize the cyw43 chip (needed to use onboard LED!)
  if(cyw43_arch_init())
  {
      std::cout << "CYW43 INIT failed!" << '\n';
      return 1;
  }
  std::cout << "CYW43 initialized!" << '\n';

  srandom(1133); //Set random number generator seed
  for (int i = 0; i < 16; i++) {
    randoms[i] = random();
    std::cout << randoms[i] << '\n';
  }
}