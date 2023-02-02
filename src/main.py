from machine import Pin
import time

led = Pin(25, Pin.OUT)
AV = Pin(2, Pin.OUT)
BV = Pin(3, Pin.OUT)
CV = Pin(4, Pin.OUT)
AG = Pin(5, Pin.OUT)
BG = Pin(6, Pin.OUT)
CG = Pin(7, Pin.OUT)

states = [1, 1, 1, 0, 0, 0]  # [0]=AV, [1]=BV, [2]=CV, [3]=AG, [4]=BG, [5]=CG


def off_all():
    AV.value(1)
    BV.value(1)
    CV.value(1)
    AG.value(0)
    BG.value(0)
    CG.value(0)
    states[0] = 1
    states[1] = 1
    states[2] = 1
    states[3] = 0
    states[4] = 0
    states[5] = 0


def coil_1_2():
    AV.value(0)
    BG.value(1)
    states[0] = 0
    states[4] = 1


def coil_1_3():
    AV.value(0)
    CG.value(1)
    states[0] = 0
    states[5] = 1


def coil_2_3():
    BV.value(0)
    CG.value(1)
    states[1] = 0
    states[5] = 1


def coil_2_1():
    BV.value(0)
    AG.value(1)
    states[1] = 0
    states[3] = 1


def coil_3_1():
    CV.value(0)
    AG.value(1)
    states[2] = 0
    states[3] = 1


def coil_3_2():
    CV.value(0)
    BG.value(1)
    states[2] = 0
    states[4] = 1


def check_short():  # check if combination shorts the circuit
    short = 0
    if (states[0] == 0 and states[3] == 1):
        print("short! 1")
        short = 1
    if (states[1] == 0 and states[4] == 1):
        print("short! 2")
        short = 1
    if (states[2] == 0 and states[5] == 1):
        print("short! 3")
        short = 1
    return short

def check_coils(): # checks which coil combination is "open"    WARNING: this do not check for shorts!
    coils = [0, 0, 0] # coil 1,2,3 (0=off 1=posetive -1=negative)
    if (states[0] == 0 and states[4] == 1):
        coils[0] = 1
        coils[1] = -1
    if (states[0] == 0 and states[5] == 1):
        coils[0] = 1
        coils[2] = -1
    if (states[1] == 0 and states[3] == 1):
        coils[1] = 1
        coils[0] = -1
    if (states[1] == 0 and states[5] == 1):
        coils[1] = 1
        coils[2] = -1
    if (states[2] == 0 and states[3] == 1):
        coils[2] = 1
        coils[0] = -1
    if (states[2] == 0 and states[4] == 1):
        coils[2] = 1
        coils[1] = -1
    return coils




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
