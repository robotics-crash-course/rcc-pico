#include <rcc_stdlib.h> // rcc_stdlib includes all the "core" necessities for the robot
using namespace std;    // Simply allows us to not have to write std::cout instead we write just cout

/** Define variables here in the global variable space
 * Do NOT give the variables values though! We can only instantiate here, not initialize
 */
int val;
double val2;

/** The main function is the ENTRY POINT for the program.
 * This is where the logic for your program starts after all the necessary startup stuff for the micro
 * processor has completed
 */
int main(void)
{
    // General pico initializations
    stdio_init_all();          // Initialize the serial communications bus (defaulted to the usb in dev/pico/CMakeLists.txt)
    sleep_ms(1000);            // I typically sleep for 1s, to let the serial interface initialize so nothing is missed in the serial terminal
    cyw43_arch_init();         // pico_w specific, will initialize the wireless chip, we need it to blink the onboard led
    cyw43_arch_gpio_put(0, 1); // turns on the onboard LED

    // Rcc specific initializations
    rcc_init_pushbutton();
    rcc_init_potentiometer();
    /** Other initializations will happen here such as i2c for example
     *
     */

    // Initialize/Instantiate variables, they will be local to main!
    int a_value;
    float another_value = 2.1;
    bool a_flag = false;
    uint32_t cur_time; // A 32-bit unsigned integer, useful for getting time

    /** For those of you who have used Arduino, this is like your void loop() function
     * Think of this as the logic that will intinitely run on the robot assuming not catastrphic errors
    */
    while (true)
    {
        cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //Flip the state of the LED
        int pot_val = adc_read(); //Read the potentiometer, notice that 
        int btn_val = gpio_get(RCC_PUSHBUTTON); //Get the state of gpio pin at RCC_PUSHBUTTON
        cout << "ADC raw val:" << pot_val << " pushbutton val: " << btn_val << "\n";

        sleep_ms(100); // sleep the processor for 50 ms
        // busy_wait_us(1000); // Waste processor cycles to wait 1000 micro seconds. Similar to sleep but not identical!

    }
}