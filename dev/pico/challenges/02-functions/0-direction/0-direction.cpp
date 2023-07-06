/*
 * 0-direction.cpp -- direction functions
 *
 * Using MotorPower(), create four functions to go forwards,
 * backwards, left, and right.  Each function should take a single input
 * parameter and set the appropriate motor(s) power. With the functions you've
 * created, see if you can make your robot boogie ;)
 */
#include "rcc_stdlib.h"

/* 
 * Declare a Motor struct called mot
 * This is done in global var space so that we do not
 * need to pass in a pointer to mot in each of our functions.
 */
Motor mot;

int main(void)
{
  //stdio and cyw43 inits!

  /* Initialize motor hardware and turn motors on
  * 20000, is the frequency we want to run the pwm generator at!
  * If you lower this value you might be able to hear the pwm "whine"
  * RCC_ENA and RCC_ENB are defined in rcc_pins.h and are the pins your 
  * ENA and ENB pins from the motor controller are connected to the PICOW
  */
  MotorInit(&mot, RCC_ENA, RCC_ENB, 20000);
  MotorsOn(&mot);

  // call your forward(), backward(), left(), and right() functions here
  // to see if they work!  It might be nice to print the current
  // function you're calling over UART to see if everything is behaving
  // as expected.

  //At the end either call MotorsOff(&mot); or set the motor's powers to 0!
}
