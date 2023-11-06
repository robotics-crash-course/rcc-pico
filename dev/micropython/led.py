import machine
import utime

led = machine.Pin("LED", machine.Pin.OUT)

led.value(1)