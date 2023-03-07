import matplotlib.pyplot as plt
from ast import For
import numpy as np
import math
import matplotlib.pyplot as plt
from pathlib import Path
import os
import pandas as pd

ROOT = os.getcwd()

path_pwm1 = Path(ROOT + "/data/pwm1.txt")
path_pwm2 = Path(ROOT + "/data/pwm2.txt")
path_pwm3 = Path(ROOT + "/data/pwm3.txt")

path_pwm_csv1 = Path(ROOT + "/data/csv/pwm1.csv")
path_pwm_csv2 = Path(ROOT + "/data/csv/pwm2.csv")
path_pwm_csv3 = Path(ROOT + "/data/csv/pwm3.csv")

read_file_pwm1 = pd.read_csv (path_pwm1)
read_file_pwm2 = pd.read_csv (path_pwm2)
read_file_pwm3 = pd.read_csv (path_pwm3)

read_file_pwm1.to_csv (path_pwm_csv1, index=None)
read_file_pwm2.to_csv (path_pwm_csv2, index=None)
read_file_pwm3.to_csv (path_pwm_csv3, index=None)