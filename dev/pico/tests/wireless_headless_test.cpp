// Helpful debug directives. They will print things to the serial monitor to aid in debugging wireless
#define DEBUG
#define SEND_DEBUG
#define RECV_DEBUG
// #define UDP_RECV_DEBUG
#define UDP_RECV_DEBUG_DEEP

#include "rcc_stdlib.h"
#include <rcc_wireless_msg_interface.h>

using namespace std;
uint32_t Ts = 1000;
float theta = 0;

/// @brief Demux for the incoming packets
/// @param p : A message in packet form
void packet_receiver(Packet p) {
    switch (p.id()) { // Swith on the id of the packet
    case 0:
        break;

    case Test_Outbound::id: {
        #ifdef DEBUG
            printf("[DEBUG]: TEST OUTBOUND RECEIVED!\n");
        #endif
        break;
    }

    case Twist::id:
    {
        Twist twist(p);
        #ifdef RECV_DEBUG
            printf("[RECV_DEBUG]: %s", twist.repr().c_str());
        #endif
        break;
    }

    case Position::id: {
        break;
    }

    case Sensor_Data::id: {
        Sensor_Data data(p);
        #ifdef RECV_DEBUG
            cout << "[RECV_DEBUG]: " << data.repr().c_str();
        #endif
        break;
    }

    case Stop::id: {
    }

    default:
        // nothing (yet, at least)
        #ifdef RECV_DEBUG
            printf("[RECV_DEBUG]: Invalid Packet | id: %u | data: %s\n", p.id(), p.data().c_str());
        #endif
        break;
    }

}

/// @brief Initialize wireless module to connect. TODO: Put other cyw43 related startup funcs here
/// @return True:success, False:failure
bool init_cyw43()
{
    //Attempt connection
    for(int i=0; i<10; i++){
        cout << "Connection attempt #" << i << "\n";
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
            cout << "Failed to connect to:" << WIFI_SSID << " with pass: " << WIFI_PASSWORD << "\n";
            ip_addr_t temp;
            ipaddr_aton("0.0.0.0", &temp);
            if(netif_list->ip_addr.addr == temp.addr) { cout << "Got useless ip addr (0.0.0.0), retrying\n"; }
            else{return false;}
        } else {
            
            return true;
        }
    }
    return false;

}

typedef struct robot_conglomerate_s{
    WirelessMsgInterface* interface;
    MPU6050* imu;
    VL53L0X* lidar;
    Left_Odom* left;
    Right_Odom* right;

}robot_conglomerate_t;

robot_conglomerate_t* default_robot_conglomerate() { 
  robot_conglomerate_t* robot = (robot_conglomerate_t*)malloc(sizeof(robot_conglomerate_t));

  WirelessMsgInterface* interface = new WirelessMsgInterface(COMP_IP, PICO_IP, TO_COMP_PORT, TO_PICO_PORT);
  interface->setup_wireless_interface();
  MPU6050* imu = new MPU6050();
  imu->begin(i2c1);

  robot->interface = interface;
  robot->imu = imu;
  return robot;
}

typedef struct robot_sensor_data_s{
    uint16_t pot;
    float wz;
}robot_sensor_data_t;

bool send_test_message(repeating_timer_t* t) {
    //Cast user data from void pointer to interface
    WirelessMsgInterface* interface = (WirelessMsgInterface*)t->user_data;
    //Create a dummy message
    Sensor_Data data;
    uint32_t time = time_us_32();
    uint32_t potval = (uint32_t)adc_read();
    data.time = (int32_t)time;
    data.theta = 1.0;
    data.wz = 2.0;
    data.potval = potval;
    data.left = 3;
    data.right = 4;
    data.dist = 5;
    //Send data over interface (pass in packet form of message)
    interface->send_msg(data.pack());
    return true;

}


int main()
{
    uint delay_length;
    repeating_timer_t send_timer;
    rcc_init_i2c();
    rcc_init_potentiometer();
    stdio_init_all();    
    sleep_ms(1000);

    //Initialize the wireless hardware
    if (cyw43_arch_init()) {
        cout << "Failed to initialise, aborting\n";
        return 1;
    }
    cout << "Initialized cyw43 arch\n";
    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    init_cyw43(); //Attempts connection to wireless access point

    //Instantiate wireless interface class
    WirelessMsgInterface interface(COMP_IP, PICO_IP, TO_COMP_PORT, TO_PICO_PORT);
    interface.setup_wireless_interface();    

    // Turn on the LED to make sure i2c devs aren't holding the bus
    cyw43_arch_gpio_put(0, 1);

    // Have send_robot_state get called every Ts seconds
    add_repeating_timer_ms(Ts, send_test_message, &interface, &send_timer);

    // Update lwip_infra ip if router gave us a diff ip and print so we can update externally
    char * address;
    interface.lwip_infra.pico_ip = netif_list->ip_addr;
    address = ipaddr_ntoa(&interface.lwip_infra.pico_ip);
    printf("This PICO's IP address is: %s\n", address);

    while(true)
    {   
        // Check if msg has come in, deserialize it, and take action dependent on which msg it is
        interface.get_msg_timeout(&packet_receiver, 10000);
        
        // Do other NON BLOCKING code here!
        sleep_ms(1000);
        cout << address << "\n";
        
    }

    // Exit program, deinit wireless module first
    cyw43_arch_deinit();
    return 0;
}