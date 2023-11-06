import machine
import utime

led_external = machine.Pin(3, machine.Pin.OUT)

while True:
    led_external.toggle()
    utime.sleep(2)