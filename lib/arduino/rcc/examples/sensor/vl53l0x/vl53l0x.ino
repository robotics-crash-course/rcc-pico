// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * vl53l0x.ino -- demonstrate the functionality of the lidar
 *
 * Copyright (C) 2020-2024 Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022-2024  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>

VL53L0X lidar; //Instantiate an instance of the lidar class

void setup()
{
    Serial.begin(9600);
    initLidar(&lidar); //Use the rcc helper func to initialize the lidar
}

void loop()
{
    uint16_t dist = getFastReading(&lidar); //Use rcc helper func to get most recent distance reading
    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.print(" mm");
    Serial.println();
}