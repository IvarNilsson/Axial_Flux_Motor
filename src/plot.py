import matplotlib.pyplot as plt
from ast import For
import numpy as np
import math
import matplotlib.pyplot as plt
from pathlib import Path
import os

ROOT = os.getcwd()
path_s1 = Path(ROOT + "/data/sin1.txt")
path_s2 = Path(ROOT + "/data/sin2.txt")
path_s3 = Path(ROOT + "/data/sin3.txt")

path_t1 = Path(ROOT + "/data/tri1.txt")

path_pwm1 = Path(ROOT + "/data/pwm1.txt")
path_pwm2 = Path(ROOT + "/data/pwm2.txt")
path_pwm3 = Path(ROOT + "/data/pwm3.txt")


# Open the file and read the data
with open(path_s1, 'r') as f:
    data_s1 = [float(x.strip()) for x in f.readlines()]

with open(path_s2, 'r') as f:
    data_s2 = [float(x.strip()) for x in f.readlines()]

with open(path_s3, 'r') as f:
    data_s3 = [float(x.strip()) for x in f.readlines()]

with open(path_t1, 'r') as f:
    data_t1 = [float(x.strip()) for x in f.readlines()]

with open(path_pwm1, 'r') as f:
    data_pwm1 = [float(x.strip()) for x in f.readlines()]

with open(path_pwm2, 'r') as f:
    data_pwm2 = [float(x.strip()) for x in f.readlines()]

with open(path_pwm3, 'r') as f:
    data_pwm3 = [float(x.strip()) for x in f.readlines()]

fig, (wave, pwm1, pwm2, pwm3) = plt.subplots(4, 1, sharex=True)

# Plot the data
wave.plot(data_s1, linewidth=0.5)
wave.plot(data_s2, linewidth=0.5)
wave.plot(data_s3, linewidth=0.5)
wave.plot(data_t1, linewidth=0.8)
wave.grid(color='gray', linestyle='-', linewidth=0.1)

pwm1.plot(data_pwm1, linewidth=0.5)
pwm2.plot(data_pwm2, linewidth=0.5)
pwm3.plot(data_pwm3, linewidth=0.5)


plt.xlabel('Sample number')
wave.set_ylabel('Amplitude')
wave.set_ylabel('Amplitude')
wave.set_ylabel('Amplitude')

wave.set_title('PWM_simulated')

plt.show()
