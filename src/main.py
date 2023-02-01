from machine import Pin
import time

led = Pin(25, Pin.OUT)
AV = Pin(2, Pin.OUT)
BV = Pin(3, Pin.OUT)
CV = Pin(4, Pin.OUT)
AG = Pin(5, Pin.OUT)
BG = Pin(6, Pin.OUT)
CG = Pin(7, Pin.OUT)


def off_all():
    AV.value(1)
    BV.value(1)
    CV.value(1)
    AG.value(0)
    BG.value(0)
    CG.value(0)

def coil_1_2():
    AV.value(0)
    BG.value(1)
    
def coil_1_3():
    AV.value(0)
    CG.value(1)

def coil_2_3():
    BV.value(0)
    CG.value(1)

def coil_2_1():
    BV.value(0)
    AG.value(1)

def coil_3_1():
    CV.value(0)
    AG.value(1)

def coil_3_2():
    CV.value(0)
    BG.value(1)

while True:

    off_all()
    coil_1_2()
    time.sleep(0.5)

    off_all()
    coil_1_3()
    time.sleep(0.5)

    off_all()
    coil_2_3()
    time.sleep(0.5)

    off_all()
    coil_2_1()
    time.sleep(0.5)

    off_all()
    coil_3_1()
    time.sleep(0.5)

    off_all()
    coil_3_2()
    time.sleep(0.5)