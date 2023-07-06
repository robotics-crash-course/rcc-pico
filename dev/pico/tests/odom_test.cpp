#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    cout << "Starting odom test!" << "\n";
    Left_Odom left;
    Right_Odom right;
    
    while(true)
    {
        sleep_ms(100);
        cout << left.getCount() << " | " << right.getCount() << "\n";
			
    }
}
