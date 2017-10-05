
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import  *
import UIComm,UIControl,UIDetail,UIOther
import matplotlib.pyplot as pl
import serial  #pip install pyserial
import sys
import time
import random #pip install random
import binascii,encodings; 
import numpy as np
from sympy.strategies.core import switch
from struct import pack,unpack

votageForwardFit  = range(20,2700,200)  
pwmForwardFit = range(200,27000,2000) 
ForwardFunc = np.polyfit(np.array(votageForwardFit),np.array(pwmForwardFit) , 2)#用2次多项式拟合
print(ForwardFunc)