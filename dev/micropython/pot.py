import machine
import utime

potentiometer = machine.ADC(28)

#conversion_factor = 3.3 / (65535)

while True:
    print(potentiometer.read_u16())
    utime.sleep(1)

