
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import  *
 
import matplotlib.pyplot as pl
import serial  #pip install pyserial
import sys
import time
import random #pip install random
import binascii,encodings; 
import numpy as np
from sympy.strategies.core import switch
from struct import pack,unpack
 
a = [1039, 1063, 1069, 1072, 1079, 1086, 1039, 1145, 1051, 1179, 1179]
b = [1179, 1010, 1069, 1072, 1079, 1086, 1039, 1145, 1051, 1179, 1179]
c = [1039, 1063, 1069, 1072, 1079, 1086, 1039, 1145, 1051, 1124, 1124]
d = [1039, 1063, 1069, 1072, 1079, 1086, 1039, 1145, 1122, 1124, 1124]
e = [1039, 1063, 1069, 1072, 1079, 1086, 1110, 1111, 1122, 1124, 1124]
a1 = sorted(a)
b1 = sorted(b)
c1 = sorted(c)
d1 = sorted(d)
e1 = sorted(e)

a11 = (a1[4]+a1[5]+a1[6])/3
b11 = (b1[4]+b1[5]+b1[6])/3
c11 = (c1[4]+c1[5]+c1[6])/3
d11 = (d1[4]+d1[5]+d1[6])/3
e11 = (e1[4]+e1[5]+e1[6])/3
print( [a11,b11,c11,d11,e11])
 
 