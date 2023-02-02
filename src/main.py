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

start = 1

# correct order: (all off first)

# first loop AV=0, BG=1 (coil 1_2)
# else AV=0, CV=0
# delay
# BG=0, CG=1 (coil 13)
# delay
# AV=1, BV=0 (coil 23)
# delay
# CG=0, AG=1 (coil 21)
# delay
# BV=1, CV=0 (coil 31)
# delay
# AG=0, BG=1 (coil 32)
# delay
# repeat!


def AV_on():  # V_on
    AV.value(0)
    states[0] = 0


def BV_on():
    BV.value(0)
    states[1] = 0


def CV_on():
    CV.value(0)
    states[2] = 0


def AV_off():  # V_off
    AV.value(1)
    states[0] = 1


def BV_off():
    BV.value(1)
    states[1] = 1


def CV_off():
    CV.value(1)
    states[2] = 1


def AG_on():  # G_on
    AG.value(1)
    states[3] = 1


def BG_on():
    BG.value(1)
    states[4] = 1


def CG_on():
    CG.value(1)
    states[5] = 1


def AG_off():  # G_off
    AG.value(0)
    states[3] = 0


def BG_off():
    BG.value(0)
    states[4] = 0


def CG_off():
    CG.value(0)
    states[5] = 0


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


def check_coils():  # checks which coil combination is "open"    WARNING: this do not check for shorts!
    coils = [0, 0, 0]  # coil 1,2,3 (0=off 1=posetive -1=negative)
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
    if (start):
        start = 0
        off_all()

        BG_on()  # 0 (only on startup)
        AV_on()  # 0 (only on startup)
    else:
        CV_off()  # 1
        AV_on()  # 1

    time.sleep(1)

    BG_off()  # 2
    CG_on()  # 2

    time.sleep(1)

    AV_off()  # 3
    BV_on()  # 3

    time.sleep(1)

    CG_off()  # 4
    AG_on()  # 4

    time.sleep(1)

    BV_off()  # 5
    CV_on()  # 5

    time.sleep(1)

    AG_off()  # 6
    BG_on()  # 6
