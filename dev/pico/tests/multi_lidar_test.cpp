#include "rcc_stdlib.h"
using namespace std;

#define ADDRESS_SECONDARY 0x31
#define ADDRESS_DEFAULT 0x34

int main()
{
    stdio_init_all();
    sleep_ms(100);
    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }

    cyw43_arch_gpio_put(0, 1);
    rcc_init_i2c();

    VL53L0X lidar[2];

    // tofSensor startup address change
    for (int i = 0; i < 2; i++) {
      gpio_init(i);
      gpio_set_dir(i, true);
      gpio_put(i, 0);
    }
    sleep_ms(1000);
    for (int i = 0; i < 2; i++) {
      lidar[i].setBus(i2c1);
      lidar[i].setTimeout(100);
      gpio_put(i, true);
      sleep_ms(500);
      if (!lidar[i].init(true)) {
        cout << "failed to init TOF sensors\n";
      }
      sleep_ms(100);
      lidar[i].setAddress(lidar[i].getAddress() + i + 1);
      sleep_ms(300);
      lidar[i].startContinuous();
    }


    /*lidar[0].setBus(i2c1);
    lidar[0].setTimeout(200);
    lidar[0].init();
    //first_lidar.startContinuous(0);

    lidar[1].setBus(i2c1);
    lidar[1].setTimeout(200);
    lidar[1].init();
    //second_lidar.startContinuous(0);

    gpio_init(0);
    gpio_init(1);

    // set as outputs
    gpio_set_dir(0, true);
    gpio_set_dir(1, true);

    // set to 0
    gpio_put(0, false);
    gpio_put(1, false);

//Activate lidar 1
    sleep_ms(100);
    // change to input (should go low)
    gpio_set_dir(1, false);
    sleep_ms(100);

    lidar[1].setAddress(ADDRESS_SECONDARY);
    sleep_ms(100);
    // set to high
    gpio_put(0, true);
    sleep_ms(100);
    */

    while(true)
    {
        sleep_ms(100);
        uint16_t dist = lidar[0].readRangeSingleMillimeters();
        printf("First Distance (mm): %hu\n", dist);
        sleep_ms(100);

        uint16_t dist_2 = lidar[1].readRangeSingleMillimeters();
        printf("Second Distance (mm): %hu\n", dist_2);
        sleep_ms(100);
    }
}