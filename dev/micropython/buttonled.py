import machine
import utime
import urandom

led = machine.Pin(3, machine.Pin.OUT)
button = machine.Pin(22, machine.Pin.IN, machine.Pin.PULL_UP)

#this function called when interrupt triggered
def button_handler(pin):
    button.irq(handler=None)
    timer_reaction = utime.ticks_diff(utime.ticks_ms(), timer_start)
    print("reaction time: " + str(timer_reaction) + " ms!")


led.value(1)
utime.sleep(urandom.uniform(5,10))
led.value(0)
timer_start = utime.ticks_ms()
button.irq(trigger=machine.Pin.IRQ_FALLING, handler = button_handler)
