// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * vl53l0x.ino -- demonstrate the functionality of the lidar
 *
 * Copyright (C) 2020-2024 Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022-2024  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>

int rising_edge_count = 0;
int signal_prev = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(A1, INPUT);
}

void loop()
{
    int signal = digitalRead(A1);
    if(signal == HIGH && signal_prev == LOW)
    {
        rising_edge_count++;
    }
    signal_prev = signal;
    Serial.print("RISING EDGE COUNT: ");
    Serial.print(rising_edge_count );
    Serial.print(" Pin Reading: ");
    Serial.println(signal);
}
