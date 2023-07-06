/*
 * 2-stack-overflow-sol.cpp -- stack overflow solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Michael Giglia <michael.a.giglia@gmail.com>
 * 
 */
#include "rcc_stdlib.h"

unsigned int fib(unsigned int n)
{
  std::cout << "0x";
  std::cout << (uintptr_t) &n;
  std::cout << ": ";
  std::cout << n << '\n';

  if (n <= 1) return n;

  return fib(n - 1) + fib(n - 2);
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

  fib(10000);
}