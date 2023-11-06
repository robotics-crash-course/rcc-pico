import machine
import utime

potentiometer = machine.ADC(28)
led = machine.PWM(machine.Pin(3))
led.freq(1000)

while True:
    led.duty_u16(potentiometer.read_u16())
