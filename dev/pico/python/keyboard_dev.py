from pynput import keyboard
from comms.messages import Twist, Sensor_Data
from comms.controller import WirelessController, WirelessInterface, packet_receive_demux
from comms.packet import Packet

def on_press(key, interface: WirelessController):
    try:
        print('alphanumeric key {0} pressed'.format(
            key.char))
        if(key.char == 'a'):
            interface.outbound.put(Twist((1.1, 2.1)).pack())

    except AttributeError:
        print('special key {0} pressed'.format(
            key))

def on_release(key, interface):
    print('{0} released'.format(
        key))
    if key == keyboard.Key.esc:
        # Stop listener
        return False

interface = WirelessController(WirelessInterface)
interface.start_inbound()
interface.start_outbound()

listener = keyboard.Listener(on_press=lambda key:on_press(key, interface), on_release=lambda key:on_release(key, interface), interface = interface)
listener.start()

while True:
    interface.packet_receive_process(packet_receive_demux)