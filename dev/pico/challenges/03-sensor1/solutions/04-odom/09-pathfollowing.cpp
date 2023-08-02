#include "rcc_stdlib.h"
using namespace std;

//global variables xD
#define rot360   260
#define rot90     65

void blink_led(int time){
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0)); //blinks LED
    sleep_ms(time);
}

// False = Left, True = right
void rotate90(Motor* motorPointer, Left_Odom* left, Right_Odom* right, bool dir) {
    int initial_count;
    int current_count;

    if (dir) {
        initial_count = right->getCount();
    } else {
        initial_count = left->getCount();
    }

    while (current_count < (initial_count + rot90)) {
        if (dir) {
            MotorPower(motorPointer, 0, 50);
            current_count = right->getCount();
        } else {
            MotorPower(motorPointer, 50, 0);
            current_count = left->getCount();
        }
        sleep_ms(100);
    }
}

void move_forward(Motor* motorPointer, Left_Odom* left, Right_Odom* right, int count) {
    int initial_left = left->getCount();
    int initial_right = right->getCount();

    int current_left, current_right;

    while ((current_left < initial_left + count) && (current_right < initial_right + count)) {
        MotorPower(motorPointer, 50, 50);
        current_left = left->getCount();
        current_right = right->getCount();
    }
}

int main()
{
    stdio_init_all();
    sleep_ms(1500);
    cyw43_arch_init();
    cyw43_arch_gpio_put(0,1);

    rcc_init_pushbutton();

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);
    printf("AFTER MOTORS");

    Left_Odom left;
    Right_Odom right;
    printf("AFTER ODOM");

    int left_count = 0;
    int right_count = 0;
    int rot360deg = 260; //number counts for 360 degree rotation
    int rot90deg = 65; //number of counts for a 90 degree rotation

    bool blinking = false;

    while(true)
    {
        left_count = left.getCount();
        right_count = right.getCount();

        if(!gpio_get(RCC_PUSHBUTTON))
        {
            move_forward(&motors, &left, &right, 200); //run for 100 counts
            sleep_ms(200);
            rotate90(&motors, &left, &right, true); //turn right
            sleep_ms(200);
            move_forward(&motors, &left, &right, 200); //run for 100 counts
            MotorPower(&motors, 0, 0); //stop
            blinking = true;
        }

        if(blinking){
            blink_led(300); //blinks!
        }
    }

}