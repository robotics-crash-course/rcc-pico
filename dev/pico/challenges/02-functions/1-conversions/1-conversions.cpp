/*
 * 1-conversions.cpp -- unit conversion functions
 *
 * Unit conversions are something that are hard to get around in the
 * world of engineering.  Since we're going to be using Odometry to determine
 * distances travelled by the bot, why don't you create some functions that can
 * convert the "ticks" value into a distance travelled.
 * We know the following about the robot and the encoder wheels.
 * Diameter = 80mm
 * Encoder Wheel # slots = 40
 * The Odom values increase for every RISING and FALLING edge!! 
 * Think about what this means. After 1 rotation how many counts will the 
 * Odom for that wheel be?
 * 
 * Use this information to create a function that will take in a # of ticks and
 * return a float representing the linear distance the wheel has travelled. 
 * Assume the wheel has not slipped!
 * 
 * Use a DUMMY_TICKS variable to pass into these functions!
 */
