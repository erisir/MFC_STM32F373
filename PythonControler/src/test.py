
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

def logMessage(flag,msg=None):# debug
    if msg is None:
        msg = 'self.error._what+ +self.error._why'
    if flag ==0:
        print(msg)
    if flag ==1:
        print(msg+'001')
logMessage(0,'dfsdf')
 