#define DEBUG
#define SEND_DEBUG
#define RECV_DEBUG

#include "rcc_stdlib.h"
#include <rcc_wireless_msg_interface.h>

// using namespace std;

void WirelessMsgInterface::packet_receiver(Packet p) {
    this->send_msg(p);
    switch (p.id()) {
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

    case Simple_Move::id: {
        Simple_Move move(p);

        Test_Outbound tout {
            move.distance,
            move.curvature,
            move.velocity
        };
        cout << tout.pack();
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

bool init_cyw43()
{
    // if (!cyw43_arch_init()) {
    //     printf("failed to initialise\n");
    //     return false;;
    // }

    // cyw43_arch_enable_sta_mode();

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

bool send_robot_state(repeating_timer_t* t)
{
    //Get all sensor data
    robot_conglomerate_t* robot = (robot_conglomerate_t*)t->user_data;
    robot->imu->update_pico();
    float wz = robot->imu->getAngVelZ();
    uint16_t dist = getFastReading(robot->lidar);
    uint16_t potval = adc_read();

    Sensor_Data data;
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

bool print_things(repeating_timer_t* t)
{
    std::cout << "Is this printing?\n";
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
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    std::cout << "Initialized cyw43 arch\n";

    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    init_cyw43();
    WirelessMsgInterface interface(IP_SEND, IP_RECV, PORT_SEND, PORT_RECV);
    interface.setup_wireless_interface();    
    MPU6050 imu;
    imu.begin(i2c1);
    VL53L0X lidar;
    rcc_init_lidar(&lidar);
    Left_Odom left;
    Right_Odom right;

    robot_conglomerate_t robot;
    robot.imu = &imu;
    robot.interface = &interface;
    robot.lidar = &lidar;
    robot.left = &left;
    robot.right = &right;
    add_repeating_timer_ms(10, send_robot_state, &robot, &send_timer);


    char * address;
    interface.lwip_infra.ip_recv = netif_list->ip_addr;
    address = ipaddr_ntoa(&interface.lwip_infra.ip_recv);
    printf("This PICO's IP address is: %s\n", address);

    while(true)
    {   
        mutex_enter_blocking(&interface.mtx);
        Packet p;
        interface.msg_stream >> p;
        inter_thread_message m(p);
        if(!interface.msg_stream)
        {   
            // printf("Pack failed!\n");  
        }
        else{
            interface.packet_receiver(p);
        }
        mutex_exit(&interface.mtx);
        sleep_ms(100);

    }

    cyw43_arch_deinit();
    return 0;
}