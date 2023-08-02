#define DEBUG
// #define SEND_DEBUG
#define RECV_DEBUG
#define UDP_RECV_DEBUG

#include "rcc_stdlib.h"
#include <rcc_wireless_msg_interface.h>

using namespace std;
uint32_t Ts = 100;
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
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("failed to connect.\n");
        printf(WIFI_SSID);
        printf("\n");
        printf(WIFI_PASSWORD);

        return false;
    } else {
        printf("Connected.\n");
        return true;
    }

}

bool timer_callback(struct repeating_timer *t)
{
    //get wireless interface pointer from user data 
    WirelessMsgInterface* interface;
    interface = (WirelessMsgInterface*)(t->user_data);
    Packet p;
    interface->send_msg(p);
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0));
    return true;
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

  WirelessMsgInterface* interface = new WirelessMsgInterface(IP_SEND, IP_RECV, PORT_SEND, PORT_RECV);
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

/// @brief Using the robot_conglomerate_t update all sensors and send Sensor_Data msg over wifi
/// @param t The repeating timer (make sure user_data has pointer to robot_conglomerate_t)
/// @return True always
bool send_robot_state(repeating_timer_t* t)
{
    //Get all sensor data
    robot_conglomerate_t* robot = (robot_conglomerate_t*)t->user_data;
    robot->imu->update_pico();
    float wz = robot->imu->getAngVelZ();
    uint16_t dist = getFastReading(robot->lidar);
    uint16_t potval = adc_read();
    theta = theta + (wz*Ts/1000.0);
    uint32_t time = time_us_32();

    Sensor_Data data;
    data.time = (int32_t)time;
    data.theta = theta;
    data.wz = wz;
    data.potval = potval;
    data.left = robot->left->getCount();
    data.right = robot->right->getCount();
    data.dist = static_cast<int32_t>(dist);
    robot->interface->send_msg(data.pack());
    std::cout << "pot val: " << potval << " dist: " << dist <<" wz: " << wz << " lcount: " << robot->left->getCount() << 
                " rcount: " << robot->right->getCount() << '\n';
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

    //Initialize the wireless device for communication
    if (cyw43_arch_init()) {
        cout << "Failed to initialise, aborting\n";
        return 1;
    }
    cout << "Initialized cyw43 arch\n";
    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    init_cyw43();

    //Create instances of all memebrs of robot_conglomerate_t
    WirelessMsgInterface interface(IP_SEND, IP_RECV, PORT_SEND, PORT_RECV);
    interface.setup_wireless_interface();    
    MPU6050 imu;
    imu.begin(i2c1);
    imu.calibrate();
    VL53L0X lidar;
    rcc_init_lidar(&lidar);
    Left_Odom left;
    Right_Odom right;

    // Turn on the LED to make sure i2c devs aren't holding the bus
    cyw43_arch_gpio_put(0, 1);

    // Put members into instance of robot_conglomerate_t
    robot_conglomerate_t robot;
    robot.imu = &imu;
    robot.interface = &interface;
    robot.lidar = &lidar;
    robot.left = &left;
    robot.right = &right;

    // Have send_robot_state get called every Ts seconds
    add_repeating_timer_ms(Ts, send_robot_state, &robot, &send_timer);

    // Update lwip_infra ip if router gave us a diff ip and print so we can update externally
    char * address;
    interface.lwip_infra.ip_recv = netif_list->ip_addr;
    address = ipaddr_ntoa(&interface.lwip_infra.ip_recv);
    printf("This PICO's IP address is: %s\n", address);

    while(true)
    {   
        // Check if msg has come in, deserialize it, and take action dependent on which msg it is
        interface.get_msg_timeout(&packet_receiver, 10000);
        
        // Do other NON BLOCKING code here!
        
    }

    // Exit program, deinit wireless module first
    cyw43_arch_deinit();
    return 0;
}