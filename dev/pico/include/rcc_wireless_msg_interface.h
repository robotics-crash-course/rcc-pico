#include "pico/cyw43_arch.h"
#include "serial-packets/packet.h"
#include "serial-packets/packet.c++"
#include "serial-packets/serialize.h"
#include "serial-packets/messages.h"
#include <stdio.h>
#include "pico/mutex.h"

#define TO_COMP_PORT 9999
#define TO_PICO_PORT 9900
#define BEACON_MSG_LEN_MAX 500
// #define IP_SEND "192.168.1.35" //Computers IP
// #define IP_RECV "192.168.1.37" //Picos IP
#define BEACON_INTERVAL_MS 100

using namespace std;

//ty cvw
struct inter_thread_message {
	inter_thread_message() = default;
	inter_thread_message(const Packet&);
	inter_thread_message(const string& m);

	Packet pack();

	string s;
    int32_t id;
};

inter_thread_message::inter_thread_message(const Packet& p) {
	s = p.data();
    id = p.id();
}

inter_thread_message::inter_thread_message(const string& s)
: s(s) {}

Packet inter_thread_message::pack() {
	return Packet(id, s);
}

/// @brief Struct of the comms lwip_infrastructure (ips, ports)
typedef struct lwip_infra_s
{
    struct udp_pcb* pcb_recv;
    struct udp_pcb* pcb_send;
    ip_addr_t pico_ip; //This is the picos ip addr
    ip_addr_t comp_ip; //this is the lapttop ip addr
    uint16_t to_pico_port;
    uint16_t to_comp_port;
} lwip_infra_t;

class WirelessMsgInterface
{
    public:
        WirelessMsgInterface(string, string, uint32_t, uint32_t);
        void setup_wireless_interface();
        bool send_msg(Packet);
        static void recv_msg(void*,              // User argument - udp_recv `arg` parameter
                           struct udp_pcb*,   // Receiving Protocol Control Block
                           struct pbuf* ,         // Pointer to Datagram
                           const ip_addr_t* ,  // Address of sender
                           u16_t );
        void* get_packet_from_queue(Packet);
        void* send_packet_to_queue(Packet);
        bool init_cyw43();
        void packet_receiver(Packet);
        void get_msg_timeout(void (*func)(Packet), uint64_t);
        bool has_packet;
        lwip_infra_t lwip_infra;
        stringstream msg_stream;
        string msg;
        mutex_t mtx;

};

WirelessMsgInterface::WirelessMsgInterface(string comp_ip, string pico_ip, uint32_t to_comp_port, uint32_t to_pico_port)
{
    this->lwip_infra.pcb_recv = udp_new();
    this->lwip_infra.pcb_send = udp_new();
    this->lwip_infra.to_comp_port = 9999;
    this->lwip_infra.to_pico_port = 9900;
    ipaddr_aton(pico_ip.c_str(), &(this->lwip_infra.pico_ip)); 
    ipaddr_aton(comp_ip.c_str(), &(this->lwip_infra.comp_ip));
    printf("DEBUG: comp_ip %s | %s\n", COMP_IP, ipaddr_ntoa(&this->lwip_infra.comp_ip));
    printf("DEBUG: pico_ip %s | %s\n", PICO_IP, ipaddr_ntoa(&this->lwip_infra.pico_ip));
    
    //Get the ip address we've been given
    ip_addr_t ipnetif = netif_list->ip_addr;
    lwip_infra.pico_ip = ipnetif;
    mutex_init(&mtx);
}

/// @brief Poll this func to get a message
/// @param func Function pointer to the function that will unpack the message
/// @param duration Timeout duration (not implemented atm)
void WirelessMsgInterface::get_msg_timeout(void (*func)(Packet), uint64_t duration) {
    if(this->has_packet){
        uint32_t ownerout;
        mutex_try_enter(&this->mtx, &ownerout);
        Packet p;
        this->msg_stream >> p;
        inter_thread_message m(p);
        if(!this->msg_stream)
        {   
            // printf("Pack failed!\n");  
        }
        else{
            func(p);
        }
        this->has_packet = false; //Update packet status
        mutex_exit(&this->mtx);
    }
    else {
        return;
    }
}

/// @brief UDP packet recieve callback
/// @param arg : User defined args as void pointer
/// @param upcb : Receiving Protocol control block
/// @param p    : Pointer to Datagram
/// @param addr : Address of sender
/// @param port : Sender port
void WirelessMsgInterface::recv_msg( void* arg,  
                           struct udp_pcb* upcb, 
                           struct pbuf* p,       
                           const ip_addr_t* addr,
                           u16_t port )          
{

    WirelessMsgInterface* interface = (WirelessMsgInterface*)arg;

    // Process datagram here (non-blocking code)
    char* tmpPtr;
    tmpPtr = reinterpret_cast<char*>((uint8_t*)(p->payload));
    char data[p->tot_len]; //char array to place udp packet charaters into
    stringstream tmp;
    // auto lock = std::unique_lock{mtx};
    // interface->readingStarted.wait(lock);
    uint32_t ownerout;
    if(mutex_try_enter(&interface->mtx, &ownerout))
    {
        interface->msg_stream.str("");
        interface->msg_stream.clear();
        for(int i = 0; i < p->len; i++)
            {   
                data[i] = *(tmpPtr++);
                tmp << data[i];
                interface->msg_stream << data[i];
            }
            // inter_thread_message m(out.pack());
            // inter_thread_message m(tmp.str());
            // interface->msg_stream << m.s;
            #ifdef UDP_RECV_DEBUG 
                printf("[UDP_RECV_DEBUG]: Received msg!\n");
            #endif
            #ifdef UDP_RECV_DEBUG_DEEP 
                cout << "[UDP_RECV_DEBUG_DEEP]:" << "ip: " << addr << " port: " << port << " data: " << data;
            #endif
            interface->has_packet = true;
            mutex_exit(&interface->mtx);
    }

    // Must free receive pbuf before return
    pbuf_free(p);
}



/// @brief Setup lwip for incoming udp packets and sending udp packets
void WirelessMsgInterface::setup_wireless_interface()
{
    //Initialize udp receive and callback
    // const ip_addr_t pico_ip =send lwip_infra.pico_ip;
    udp_bind(lwip_infra.pcb_recv, &lwip_infra.pico_ip, lwip_infra.to_pico_port); //Bind the pico ipaddr to port 9990
    udp_recv(lwip_infra.pcb_recv, this->recv_msg, this); //Setup recv callback fcn
}

/// @brief Send a message but in packet form
/// @param pack The message as a Packet to be send
/// @return True if successful, False (not implemented atm)
bool WirelessMsgInterface::send_msg(Packet pack)
{
    Packet packet;
    packet = pack;

    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, BEACON_MSG_LEN_MAX+1);
    // string s("TESTING");
    // snprintf(req, BEACON_MSG_LEN_MAX, "%x%s%x\r\n", start_tx, s.c_str(), end_tx); 
    stringstream s;
    s << packet;
    snprintf(req, BEACON_MSG_LEN_MAX, "%s", s.str().c_str()); //instead get the package stsring

    err_t er = udp_sendto(lwip_infra.pcb_send, p, &lwip_infra.comp_ip, TO_COMP_PORT); //Send the string over udp
    pbuf_free(p);

    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d", er);
    } else {
        // printf("Sent packet %s\n", packet.data().c_str()); 
        #ifdef SEND_DEBUG
            cout << "[SEND_DEBUG]: Sent: " << packet << '\n';
        #endif
    }
    return true;
}
