// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom.ino -- demonstrate the functionality of the encoders
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */

#include <rcc.h>

#define CPR 48
#define N 48.0

PID_control_config_t pid_config;

PID_control* lctrl;

Left_Dir_Odom odom;

unsigned long cur, prev;
unsigned long dt = 10000;

void setup()
{
  Serial.begin(115200);

  pid_config.kp = 10;
  pid_config.ki = 0.0;
  pid_config.kd = 1;
  pid_config.lowerLimit = -150;
  pid_config.upperLimit = 150;
  pid_config.sigma = 0.1;
  pid_config.ts = dt/1000000.0;
  pid_config.antiWindupEnabled = true;

  lctrl = new PID_control(pid_config);
  lctrl->setDeadbands(-60,60);
  motorSetup();
}

void loop()
{  
  cur = micros();
  if(cur-prev >= dt){
    long count_current = odom.getCount();
  
    //Position control
    float theta = (count_current/(CPR * N)) * 360;
    float pwr = lctrl->pid(90, theta);
    rawMotorCtrl(pwr,0);
    
    prev=cur;
    
    Serial.print("theta: ");
    Serial.print(theta);
    Serial.print(" | pwr: ");
    Serial.println(pwr);
  }
}
