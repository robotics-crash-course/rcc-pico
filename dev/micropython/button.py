import machine
import utime

button = machine.Pin(22, machine.Pin.IN, machine.Pin.PULL_UP)

while True:
    if button.value() == 0:
        print("pressed")
        utime.sleep(200)