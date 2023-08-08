#include "rcc_stdlib.h"
using namespace std;

// Creating a simple file to make calculations for triangles!

int hypotenuse;

int find_hypotenuse(int a, int b) {
    double c = sqrt(a*a + b^^2);
}

void print_triangle(int a, int b, int c) {
    print("a: %d", a);
    printf("b: %f", b);
    cout << "c: " << c;
}

int main(void)
{
    stdio_init_all();

    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }

    cyw43_arch_gpio_put(0, true);

    int a = 3;
    int b = 4;
    while(true)
    {
        cyw43_arch_gpio_put(0, !(cyw43_arch_gpio_get(0)));

        hypotenuse = find_hypotenuse(a, b);
        print_triangle(a, b, c);

    }
}
