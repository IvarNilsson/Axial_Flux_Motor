from machine import Pin
import time

led = Pin(25, Pin.OUT)
AV = Pin(2, Pin.OUT)
BV = Pin(3, Pin.OUT)
CV = Pin(4, Pin.OUT)
AG = Pin(5, Pin.OUT)
BG = Pin(6, Pin.OUT)
CG = Pin(7, Pin.OUT)

while True:
    led.value(1)
    time.sleep(0.5)
    led.value(0)
    time.sleep(0.5)
