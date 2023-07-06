/*
 * 2-generalization-sol.ino -- array generalization solution
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Michael Giglia <michael.a.giglia@gmail.com>
 */

#include "rcc_stdlib.h"


int left_motor[]  = {100,    0,  200, -100,    0,   0,   50,  200, -200, -150,  200, -50};
int right_motor[] = {100,  200,    0, -100, -200,   0,  -25,  200, -200,  150, -200, -50};
int delays[]      = {800, 1000, 1000,  800, 2000, 500, 4000, 2000, 2000, 1000, 4000, 800};

int *dance[] = {left_motor, right_motor, delays};


void print_2d_array(int **a, int n, int m)
{
  for (int i = 0; i < n; i++) {
    for (int j = 0; i < m; j++) {
      std::cout << "[";
      std::cout << i;
      std::cout << ", ";
      std::cout << j;
      std::cout << "] = ";
      std::cout << a[i][j] << '\n';
    }
  }
}

int main()
{
  stdio_init_all();
  sleep_ms(1500);
  
  //Initialize the cyw43 chip (needed to use onboard LED!)
  if(cyw43_arch_init())
  {
      std::cout << "CYW43 INIT failed!"  ;
      return 1;
  }
  std::cout << "CYW43 initialized!"  ;

  //aka print_2d_array(dance, 3, 12);
  print_2d_array(dance, ARRAYSIZE(dance), ARRAYSIZE(left_motor));
}
