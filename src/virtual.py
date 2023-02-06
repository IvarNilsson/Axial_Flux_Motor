import time

led = 0
AV = 0
BV = 0
CV = 0
AG = 0
BG = 0
CG = 0

states = [1, 1, 1, 0, 0, 0]  # [0]=AV, [1]=BV, [2]=CV, [3]=AG, [4]=BG, [5]=CG

start = 1


def AV_on():  # V_on
    AV = 0
    states[0] = 0


def BV_on():
    BV = 0
    states[1] = 0


def CV_on():
    CV = 0
    states[2] = 0


def AV_off():  # V_off
    AV = 0
    states[0] = 1


def BV_off():
    BV = 0
    states[1] = 1


def CV_off():
    CV = 0
    states[2] = 1


def AG_on():  # G_on
    AG = 0
    states[3] = 1


def BG_on():
    BG = 0
    states[4] = 1


def CG_on():
    CG = 0
    states[5] = 1


def AG_off():  # G_off
    AG = 0
    states[3] = 0


def BG_off():
    BG = 0
    states[4] = 0


def CG_off():
    CG = 0
    states[5] = 0


def off_all():
    AV = 1
    BV = 1
    CV = 1
    AG = 0
    BG = 0
    CG = 0
    states[0] = 1
    states[1] = 1
    states[2] = 1
    states[3] = 0
    states[4] = 0
    states[5] = 0
    if (seems_good() != 2):
        print("ERROR0", seems_good())


def coil_1_2():
    AV = 0
    BG = 1
    states[0] = 0
    states[4] = 1
    if (seems_good() != 1):
        print("ERROR1", seems_good())


def coil_1_3():
    AV = 0
    CG = 1
    states[0] = 0
    states[5] = 1
    if (seems_good() != 1):
        print("ERROR2", seems_good())


def coil_2_3():
    BV = 0
    CG = 1
    states[1] = 0
    states[5] = 1
    if (seems_good() != 1):
        print("ERROR3", seems_good())


def coil_2_1():
    BV = 0
    AG = 1
    states[1] = 0
    states[3] = 1
    if (seems_good() != 1):
        print("ERROR4", seems_good())


def coil_3_1():
    CV = 0
    AG = 1
    states[2] = 0
    states[3] = 1
    if (seems_good() != 1):
        print("ERROR5", seems_good())


def coil_3_2():
    CV = 0
    BG = 1
    states[2] = 0
    states[4] = 1
    if (seems_good() != 1):
        print("ERROR6", seems_good())


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


def seems_good():  # auto checks (0=bad 1=good 2=off)
    short = check_short()
    if (short == 1):
        return 0

    coils = check_coils()
    if (coils[0] == 0 and coils[1] == 0 and coils[2] == 0):
        return 2
    elif ((coils[0] + coils[1] + coils[2]) != 0):
        print("????")
        return 0
    else:
        return 1


for i in range(5):
    print("loop", i)

    if (start):
        start = 0
        off_all()
        if (seems_good() != 2):
            print("ERROR", seems_good())

        BG_on()  # 0 (only on startup)
        AV_on()  # 0 (only on startup)
        if (seems_good() != 1):
            print("ERROR", seems_good())
    else:
        CV_off()  # 1
        AV_on()  # 1
        if (seems_good() != 1):
            print("ERROR", seems_good())

    time.sleep(1)

    BG_off()  # 2
    CG_on()  # 2
    if (seems_good() != 1):
        print("ERROR", seems_good())

    time.sleep(1)

    AV_off()  # 3
    BV_on()  # 3
    if (seems_good() != 1):
        print("ERROR", seems_good())

    time.sleep(1)

    CG_off()  # 4
    AG_on()  # 4
    if (seems_good() != 1):
        print("ERROR", seems_good())

    time.sleep(1)

    BV_off()  # 5
    CV_on()  # 5
    if (seems_good() != 1):
        print("ERROR", seems_good())

    time.sleep(1)

    AG_off()  # 6
    BG_on()  # 6
    if (seems_good() != 1):
        print("ERROR", seems_good())

    time.sleep(1)
    