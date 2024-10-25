// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * rcc.h -- robotics crash course
 *
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_H
#define CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_H


#include "actuator/motor.h"
#include "actuator/servo.h"
#include "Wire.h"
#include "sensor/hc-sr04.h"
#include "sensor/mpu6050.h"
#include "VL53L0X.h"
#include "sensor/odom.h"
#include "util/array.h"
#include "util/differentiator.h"
#include "util/pid-control.h"

void initLidar(VL53L0X* sensor)
{
  Wire.begin();

  sensor->setTimeout(200);
  if (!sensor->init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor->startContinuous(0);

}

uint16_t getFastReading(VL53L0X* sensor)
{

  uint16_t range = sensor->readReg16Bit(sensor->RESULT_RANGE_STATUS + 10);
  //  sensor.writeReg(sensor.SYSTEM_INTERRUPT_CLEAR, 0x01);
  return range;
}

void i2c_bus_recovery()
{
        for(int i=0; i<16; i++){
        digitalWrite(A4, true);
        delay(10);
        digitalWrite(A4, false);
        delay(10);
    }
}

#endif /* CU_SUMMER_STEM_ROBOTICS_CRASH_COURSE_RCC_H */
