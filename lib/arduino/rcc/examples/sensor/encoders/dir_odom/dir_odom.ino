// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * odom.ino -- demonstrate the functionality of the encoders
 *
 * Copyright (C) 2020-2021  Michael Giglia <michael.a.giglia@gmail.com>
 * Copyright (C) 2022  Jacob Koziej <jacobkoziej@gmail.com>
 * Copyright (C) 2023  Catherine Van West <cat@vanwestco.com>
 */

#include <rcc.h>

Right_Dir_Odom rodom;
Left_Dir_Odom lodom;

void setup()
{
  Serial.begin(9600);

}

void loop()
{
    int lcount = lodom.getCount();
    int rcount = rodom.getCount();

    Serial.print("lcount: ");
    Serial.print(lcount);
    Serial.print(" | rcount: ");
    Serial.print(rcount);
    Serial.println();
}