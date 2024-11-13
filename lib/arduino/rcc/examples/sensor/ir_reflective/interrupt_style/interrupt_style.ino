// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * vl53l0x.ino -- demonstrate the functionality of the lidar
 *
 * Copyright (C) 2020-2024 Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022-2024  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <rcc.h>
#include <PinChangeInterrupt.h>

volatile int isr_count = 0;

void setup()
{
    Serial.begin(9600);
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(A1), ir_isr, RISING);
}

void loop()
{
    Serial.print("ISR Count: ");
    Serial.println(isr_count);
}

void ir_isr(void)
{
    isr_count++;
    digitalWrite(13, !digitalRead(13));
}